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
#include <mqueue.h>
#include "update.h"

void update(){
    
    printf("daemon Child process");
    char* copy_cmd = {"cp -u -a /assignment/intra/. /assignment/live"};
    if (system(copy_cmd)==-1){
        mqd_t mq;
        char buffer[1024];
        mq=mq_open("/Queue",O_WRONLY);
        mq_send(mq,"update_failed",1024,0);
        mq_close(mq);
        openlog("Assignment",LOG_PID|LOG_CONS,LOG_USER);
        syslog(LOG_INFO,"update failed");
        closelog();
    }else{
        mqd_t mq;
        char buffer[1024];
        mq=mq_open("/Queue",O_WRONLY);
        mq_send(mq,"update_successful",1024,0);
        mq_close(mq);
        openlog("Assignment",LOG_PID|LOG_CONS,LOG_USER);
        syslog(LOG_INFO,"update successful");
        closelog();
    }
    
}