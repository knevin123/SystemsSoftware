#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void main(){
    int fd;
    char * fifoFile="/tmp/fifoFile";
    
    mkfifo(fifoFile,0666);
    
    fd= open(fifoFile,O_WRONLY);
    
    write(fd,"Transfer",sizeof("Transfer"));
    
    
    close(fd);
    unlink(fifoFile);
    
}