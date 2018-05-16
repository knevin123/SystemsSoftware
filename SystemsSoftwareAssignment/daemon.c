#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include "locking.h"
#include <mqueue.h>
#include "backup.h"
#include "update.h"
#include "updateAudit.h"

int status;

int main(){
    time_t now;
    struct tm newyear;
    double seconds;
    time(&now);  /* get current time; same as: now = time(NULL)  */
    newyear = *localtime(&now);
    newyear.tm_hour = 21;
    newyear.tm_min = 15;
    newyear.tm_sec = 0;

    int ppid;

    int pid = fork();

    if(pid>0){
    printf("Parent process");
    //sleep(10);
    exit(EXIT_SUCCESS);
    }else if (pid==0){
    while(1){
        printf("Child process");

        if (setsid() < 0) { exit(EXIT_FAILURE); }

        umask(0);

        if (chdir("/") < 0 ) { exit(EXIT_FAILURE); }

        int x;
        for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
        {
         close (x);
        }

        while(1) {
            sleep(1);
            FILE *pFile;
            pFile=fopen("/assignment/logfile.txt", "a+");
            updateAudit();
           //printf("child 1: my parent is: %i\n", getppid());
            time(&now);
            seconds = difftime(now,mktime(&newyear));
            printf("\n%.f", seconds);
            if (seconds == 0) {
                 //fprintf(pFile,"done!\n",seconds);
                 lock();
                 backup();
                 sleep(5);
                 update();
                 unlock();
            }else{
                fprintf(pFile,"%f,",seconds);
            }
            //setting up pipe to make ondemand 
            //backup and transfer  
            int f;
            char * fifoFile="/tmp/fifoFile";
            char buf[1024]="";
            //mkfifo(fifoFile,0666);  

            f=open(fifoFile,O_RDONLY);
            read(f,buf,1024);  
            if(strcmp(buf,"Copy")==0){
                lock();
                backup();
                unlock();

            }
            if(strcmp(buf,"Transfer")==0){
                lock();
                update();
                unlock();

            }
            //unlink(fifoFile);  
            close(f);
            char buffer[256];
            ppid = getpid();
            if(pFile==NULL) {
                perror("Error opening file.");
                kill(ppid , SIGTERM);
            }
            else {
                //fprintf(pFile, "%s", "Jon");
                //kill(ppid , SIGTERM); // uncomment to simulate error
            }
            fclose(pFile);
          }
        }

    }
  return 0;
}
