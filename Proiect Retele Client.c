
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "MyTest_FIFO"

int main()
{
    char input[300];
    int num, fd;

    printf("Server Started\n");
    fd = open(FIFO_NAME, O_WRONLY);

    printf("User Connected. Use the following commands:\n");
    printf("login : (username), get logged-users ,logout, get proc info, quit");

    while (gets(input), !feof(stdin)) 
    {
        if ((num = write(fd, input, strlen(input))) == -1)
            perror("Problema la scriere in FIFO!");
        else
        {
            //printf("S-au scris in FIFO %d bytes\n", num);
            
        }
            
    }
}