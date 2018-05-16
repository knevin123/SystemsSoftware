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
#include "locking.h"

void lock(){
    char mode[] = "0700";
    char buf[100] = "/assignment";
    int i;
    i = strtol(mode, 0, 8);
        
    if (chmod (buf,i) < 0)
    {
        mqd_t mq;
        char buffer[1024];
        mq=mq_open("/Queue",O_WRONLY);
        mq_send(mq,"lock_failed",1024,0);
        mq_close(mq);
        openlog("Assignment",LOG_PID|LOG_CONS,LOG_USER);
        syslog(LOG_INFO,"folder lock failed");
        closelog();
    }else{
        mqd_t mq;
        char buffer[1024];
        mq=mq_open("/Queue",O_WRONLY);
        mq_send(mq,"lock_success",1024,0);
        mq_close(mq);
        openlog("Assignment",LOG_PID|LOG_CONS,LOG_USER);
        syslog(LOG_INFO,"folder lock sucessful");
        closelog();
    }
}
void unlock(){
    char mode[] = "0777";
    char buf[100] = "/assignment";
    int i;
    i = strtol(mode, 0, 8);
        
    if (chmod (buf,i) < 0)
    {
        mqd_t mq;
        char buffer[1024];
        mq=mq_open("/Queue",O_WRONLY);
        mq_send(mq,"unlock_failed",1024,0);
        mq_close(mq);
        openlog("Assignment",LOG_PID|LOG_CONS,LOG_USER);
        syslog(LOG_INFO,"folder unlock failed");
        closelog();
    }else{
        mqd_t mq;
        char buffer[1024];
        mq=mq_open("/Queue",O_WRONLY);
        mq_send(mq,"unlock_success",1024,0);
        mq_close(mq);
        openlog("Assignment",LOG_PID|LOG_CONS,LOG_USER);
        syslog(LOG_INFO,"folder unlock sucessful");
        closelog();
    }
}