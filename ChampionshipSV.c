#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sqlite3.h>

// Define structure for a user, which contains a username and password
typedef struct 
{
    char username[32];
    char password[32];
    int tip;
} user_t;

// Global variable for SQLite database connection
sqlite3 *db;

// Global mutex for protecting shared resources
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function for connecting to SQLite database
int connect_to_db() 
{
    int rc = sqlite3_open("users.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    return 1;
}

// Function for creating a table for storing user data
int create_user_table() 
{
    char *sql = "CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT NOT NULL, tip TEXT NOT NULL)";
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error creating table: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }
    return 1;
}
//TODO make a function in c that checks if the user is in the SQLite database and the password and username match with a given input 

//TODO make a function that "logs in" the user

//TODO make a function that allows the admins only can create a tournament

//TODO make a function that allows users to register to a tournament and get a response via email

//TODO make a function that allows the admins only to delete a tournament

//TODO make a function that allows users to reschedule a tournament


// Function for inserting a new user into the table
int insert_user(user_t user) 
{
    // Use prepared statements for inserting user data to protect against SQL injection attacks
    sqlite3_stmt *stmt;
    char *sql = "INSERT INTO users (username, password, tip) VALUES (?, ?, ?)";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);

                return 1;
    }

    // Bind values to prepared statement
    sqlite3_bind_text(stmt, 1, user.username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user.password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user.tip, -1, SQLITE_STATIC);

    // Execute prepared statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error inserting user: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    // Finalize prepared statement
    sqlite3_finalize(stmt);
    return 1;
}

// Thread function for handling client connections
void *client_handler(void *arg) 
{
    int client_fd = *(int*)arg;

    // Lock mutex
    pthread_mutex_lock(&mutex);

    // Prompt user for username, password and type
    user_t user;
    printf("Enter username: ");
    scanf("%s", user.username);
    printf("Enter password: ");
    scanf("%s", user.password);
    printf("Enter user type ( 1-regular,2-admin)");
    scanf("%s",user.tip)

    // Insert user into database
    if (insert_user(user)) {
        printf("Successfully registered user %s\n", user.username);
    } else {
        fprintf(stderr, "Error registering user %s\n", user.username);
    }

    // Unlock mutex
    pthread_mutex_unlock(&mutex);

    // Close client socket
    close(client_fd);
    return NULL;
}

int main(int argc, char *argv[]) 
{
    // Connect to SQLite database
    if (!connect_to_db()) 
    {
        return 1;
    }

    // Create table for storing user data
    if (!create_user_table()) 
    {
        return 1;
    }

    // Create server socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) 
    {
        perror("Error creating socket");
        return 1;
    }

    // Bind socket to local address
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(12345);
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Error binding socket");
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 10) < 0) 
    {
        perror("Error listening for connections");
        return 1;
    }

    printf("Waiting for connections...\n");

    // Loop for handling incoming connections
    while (1) 
    {
        // Accept incoming connection
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len)

        if (client_fd < 0) 
        {
            perror("Error accepting connection");
            continue;
        }

        // Create thread for handling client connection
        pthread_t thread;
        int *arg = malloc(sizeof(*arg));
        *arg = client_fd;
        if (pthread_create(&thread, NULL, client_handler, arg) != 0) 
        {
            perror("Error creating thread");
            continue;
        }
    }

    // Close server socket
    close(server_fd);
    return 0;
}

/*
This code creates a multithreaded server that listens for 
incoming connections on port 12345. 
When a connection is received, it creates
a new thread to handle the client and prompts the
user to enter a username and password.
The user data is then inserted into 
an SQLite database.
 */