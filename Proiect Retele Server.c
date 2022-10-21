#include <stdio.h> 
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#define LENGTH 1000

int availableusers(char username)
{
    FILE *textfile;
    char line[LENGTH];
    int gasit=0;

    textfile = fopen("utilizatori.txt","r");
    if(textfile == NULL ) return 1;//trb schimbat
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

void comenzi()
{
    pid_t pid;
    int ok;

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
              char comenzidisp[][40]={'get logged-users ,   logout',
                                      'get proc info    ,   quit  '};
              fgets(citire,100,stdin);
              if(strcmp(citire,"get logged-users")==1)
              {
                switch (pid=fork())
                { 
                    case -1:            // eroare la fork
                    perror("fork");  
                    exit(1); 
                
                    case 0:             // informatii 
              
                    default:
                
                }
              }
              if(strcmp(citire,"get-proc-info")==1)
              {
                
              }
              if(strcmp(citire,"logout")==1)
              {
                
              }
              if(strcmp(citire,"quit")==1)
              {
                
              }
              switch (pid = fork())
              {
              case -1:            // eroare la fork
               perror("fork");  
               exit(1); 
                
              case 0:
                //utilizatorul este logat, urmeaza celelalte comenzi
              
              default:
                
              }
              exit(ok);// proces de iesire   
            } 

        default:            // proces parinte
            
            sleep(60);// asteapta un exit 
            printf("Exiting the program, took to long to authenticate ...\n");
            exit(0);
            
            
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
            //printf("S-au citit din FIFO %d bytes: \"%s\"\n", num, s);

        }
    } while (num > 0);
}