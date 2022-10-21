#include <stdio.h> 
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <utmp.h>
#define LENGTH 1000

int availableusers(char username)
{
    FILE *textfile;
    char line[LENGTH];
    int gasit=0;

    textfile = fopen("utilizatori.txt","r");
    if(textfile == NULL ) return 0;//trb schimbat
    while(fgets(line,LENGTH,textfile))
    {
        if(strcmp(line,username)==1) 
        {
            gasit=1;
            break;
        }
    }
    if(gasit==1)return 1;
    else printf("Utilizatorul nu exita!");
}

 void utilizatorilogati(struct utmp *u) 
 {
       
        //ut_user,ut_host,ut_tv.tv_sec
        
        //username, hosname for remote login, time entry was made

        while(u!=NULL)
        { 
            u = getutent();
            printf("USER: %s \n HOSTNAME %s \n TIME ENTRY: ", u->ut_user, u->ut_host);
            time_t time;
            time=ut_tv.tv_sec;
            printf("%25s", ctime(&time));
        }

        
}

void comenzi()
{
    pid_t pid;
    int ok,iesire=0;

    switch(pid=fork()) 
    {
        case -1:            // eroare la fork
            perror("fork");  
            exit(1);        

        case 0:             // proces copil! LOGIN
            char *citire;
            fgets(citire,100,stdin);
            char * pch;
            int gasit=0;
            while(gasit!=0)
            {
                pch = strtok (citire,":");
                if(strcmp(pch,"login :")==1)
                {
                    pch = strtok(pch,"\n");
                    if(availableusers(pch)==1)
                    {
                        gasit=1;
                        break;
                    }
                }
                fgets(citire,100,stdin);
            }

            if(gasit==1)
            {
              printf("Utilizator logat!");
              char comenzidisp[][40]={'get-logged-users ,   logout',
                                      'get proc info    ,   quit  '};
              fgets(citire,100,stdin);
              int logout;
              switch (pid=fork())
                { 
                    case -1:            // eroare la fork
                        perror("fork"); 
                        exit(1); 

                    case 0:             // informatii 
                    if(strcmp(citire,"get-logged-users")==1)
                        {
                            struct utmp *entry;
                            utilizatorilogati(entry);
                            
                        }
              
                    /*
                    if(strcmp(citire,"get-proc-info : pid")==1)
                        {
                            
                        }
                    */
                    if(strcmp(citire,"logout")==1)
                        {
                            logout=1;
                        }
                    if(strcmp(citire,"quit")==1)
                        {
                            printf("Exiting the program...");
                            
                        }
               /* switch (pid = fork())
              {
                case -1:            // eroare la fork
                perror("fork");  
                exit(1); 
                
                case 0:
                //utilizatorul este logat, urmeaza celelalte comenzi
              
                 
                
              }*/ 
                default: 
                    exit(&logout);// proces de iesire   
                } 
            }
        default:
                  // proces parinte
        if(iesire=1)
        {
            printf("Exiting the program ...\n");
            exit(0);
        }
        
        else
        {
            sleep(60);// asteapta un exit 
            printf("Exiting the program, took to long to authenticate ...\n");
            exit(0);
        }    
        
            
            
    }
}

#define FIFO_NAME "PROIECT"

int main()
{
    char input[300];
    int num, fd;

    
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);

    printf("Program Started!\n");
    fd = open(FIFO_NAME, O_RDONLY);
    printf("Someone has connected:\n");

     do {
         if ((num = read(fd, input, 300)) == -1)
                perror("Eroare la citirea din FIFO!");
        else {
            input[num] = '\0';
            comenzi();
            //printf("S-au citit din FIFO %d bytes: \"%s\"\n", num, s);

        }
    } while (num > 0);
}