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
#include "updateAudit.h"

void updateAudit(){
    
    printf("daemon Child process");
    char* copy_cmd = {"ausearch -f /assignment/intra> /assignment/accesslog.txt"};
    if (system(copy_cmd)==-1){
        mqd_t mq;
        char buffer[1024];
        mq=mq_open("/Queue",O_WRONLY);
        mq_send(mq,"update_failed",1024,0);
        mq_close(mq);
        openlog("Assignment",LOG_PID|LOG_CONS,LOG_USER);
        syslog(LOG_INFO,"update audit failed");
        closelog();
    }else{
        mqd_t mq;
        char buffer[1024];
        mq=mq_open("/Queue",O_WRONLY);
        mq_send(mq,"update_successful",1024,0);
        mq_close(mq);
        openlog("Assignment",LOG_PID|LOG_CONS,LOG_USER);
        syslog(LOG_INFO,"update audit successful");
        closelog();
    }
        
}