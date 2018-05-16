#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <syslog.h>
#include <string.h>

int main(){
    /*
    int ppid;

    int pid = fork();

    if(pid>0){
        printf("Parent process");
        //sleep(10);
        exit(EXIT_SUCCESS);
    }else if (pid==0){
        printf("Child process");

        sleep(1);

        if (setsid() < 0) { exit(EXIT_FAILURE); }

        umask(0);

        if (chdir("/") < 0 ) { exit(EXIT_FAILURE); }

        int x;
        for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
        {
            close (x);
        }

        mqd_t mq;
        struct mq_attr queue_attributes;
        char buffer[1024+1];
        int terminate=0;

        queue_attributes.mq_flags=0;
        queue_attributes.mq_maxmsg=10;
        queue_attributes.mq_msgsize=1024;
        queue_attributes.mq_curmsgs=0;

        mq=mq_open("/Queue",O_CREAT | O_RDONLY, 0644, &queue_attributes);
        
        FILE *pFile;
        pFile=fopen("/assignment/messageQ.txt", "a+");
        do{
            char mode[] = "0777";
            char buf[100] = "/assignment/messageQ.txt";
            int i;
            i = strtol(mode, 0, 8);

            if (chmod (buf,i) < 0)
            {
                // do something if needed
            }
            ssize_t bytes_read;
            bytes_read=mq_receive(mq,buffer,1024,NULL);

            buffer[bytes_read]='\0';
            fputs(buffer,pFile);
        }while(!terminate);
        mq_close(mq);
        mq_unlink("/queue");
    }*/
    mqd_t mq;
    struct mq_attr queue_attributes;
    char buffer[1024+1];
    int terminate=0;

    queue_attributes.mq_flags=0;
    queue_attributes.mq_maxmsg=10;
    queue_attributes.mq_msgsize=1024;
    queue_attributes.mq_curmsgs=0;

    mq=mq_open("/Queue",O_CREAT | O_RDONLY, 0644, &queue_attributes);

    FILE *pFile;
    pFile=fopen("/assignment/messageQ.txt", "a+");
    do{
        char mode[] = "0777";
        char buf[100] = "/assignment/messageQ.txt";
        int i;
        i = strtol(mode, 0, 8);

        if (chmod (buf,i) < 0)
        {
            // do something if needed
        }
        if(!strncmp(buffer,"exit",strlen("exit"))){
            terminate=1;
        }else{
            ssize_t bytes_read;
            bytes_read=mq_receive(mq,buffer,1024,NULL);
            printf("Received:%s\n",buffer);
            buffer[bytes_read]='\0';
            fputs(buffer,pFile);
        }
    }while(!terminate);
    mq_close(mq);
    mq_unlink("/Queue");
  return 0;
}