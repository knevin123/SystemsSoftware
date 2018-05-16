#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

//mutex to prevent file locking
pthread_mutex_t lock_x;

void *threadLogic(void *socket_desc){
    int sock=*(int*)socket_desc;
    printf("_______>%d",sock);
    fflush(stdout);
    fflush(stdin);
    int READSIZE;
    int userFound=0;
    char message[500];
    char log[500];
    char user[500];
    char filepath[200];
    memset(message,0,500);
    while((READSIZE=recv(sock,message,500,0))>0){
        fflush(stdout);
        fflush(stdin);
        if(strcmp("login",message)==0){
            memset(user,0,500);
            memset(message,0,500);
			//recieve user data
            recv(sock,user,500,0);
			//open user details file
            const char *filename = "users.txt";
            FILE *file = fopen(filename,"r");
            char line[256];
			//compare the userdata to the line in the file
            while(fgets(line,sizeof(line),file)){
                //if user found then say the user was found 
				if(strcmp(line,user)==0){
                    userFound=1;
                }
            }
			//if user found then send sucess message to the client
            if(userFound==1){
                send(sock,"200",strlen("200"),0);
                //printf("user success");
                fflush(stdout);
                fflush(stdin);
            }else{
				//if no users send a fail message to the client
                send(sock,"user fail",strlen("user fail"),0);
                printf("userfail");
            }
            printf("\nlogin done");
            fflush(stdout);
            fflush(stdin);
        }
        if(strcmp("transfer",message)==0){
            printf("\ntransfer start\n");
            fflush(stdout);
            fflush(stdin);
			//receive file from client
            char file_buffer[512];
			//base file path
            char filename[400]= "/home/keith/Desktop/Assignment2/serverfiles/";
            //recieve the file path from the client
			recv(sock,filepath,200,0);
			//add the client path onto the base path
            strcat(filename,filepath);
            send(sock,"start transfer",strlen("start transfer"),0);
            //locked the shared resource
            pthread_mutex_lock(&lock_x);
			//open file to write to
            FILE *file_open=fopen(filename,"w");
            if(file_open==NULL){
                printf("File %s Cannot be opened file on server.\n",filename);
                fflush(stdout);
                fflush(stdin);
            }else{
				
                bzero(file_buffer,512);
                int block_size=0;
                int i=0;
				//revieve file block from client
                while((block_size=recv(sock,file_buffer,512,0))>0){
                    printf("Data Received %d=%d\n",i,block_size);
                    fflush(stdout);
                    fflush(stdin);
                    //write data to file
					int write_sz=fwrite(file_buffer,sizeof(char),block_size,file_open);
                    memset(file_buffer,0,sizeof(file_buffer));
                    bzero(file_buffer,512);
                    i++;
                    if(write_sz==0||write_sz!=512){
                        break;
                    }
                }
                fclose(file_open);
                printf("Transfer Complete!\n");
                fflush(stdout);
                fflush(stdin);
				//send complete message
                send(sock,"Done",strlen("Done"),0);
                
				
				char logfile[400]= "/home/keith/Desktop/Assignment2/serverfiles/log.txt";
                //get data using time.h library
				time_t now=time(0);
                char date[26];
                struct tm *tminfo;
                tminfo=gmtime(&now);
				//write data to string
                strftime(date,sizeof(date), "%Y-%m-%d %H:%M:%S",tminfo);
				//put all logging info into log string
                strcat(log,"\n-----------------------------\n");
                strcat(log,"Username:\n");
                const char s[2]=",";
                char *token;
                //get username from user info
                token=strtok(user,s);
                strcat(log,token);
				//write file name
                strcat(log,"\nFile Changed:\n");
                strcat(log,filename);
				//write date
                strcat(log,"\nDate and Time:\n");
                strcat(log,date);
                //open log file and write to it
                FILE *log_file=fopen(logfile,"a");
                if(log_file==NULL){
                    printf("Error opening log file..\n");
                }else{
                    fputs(log,log_file);
                }
                fclose(log_file);
                memset(log,0,sizeof(log));
                //getchar();
                //unlock shared files
                pthread_mutex_unlock(&lock_x);
            }
        }
        
    }
}
int main(int argc,char *argv[])
{
    //init server variable
    int s;
    int cs;
    int *new_socket;
    int connSize;
    int READSIZE;
    int userFound=0;
    struct sockaddr_in server, client;
    char message[500];
    
    //create socket
    s=socket(AF_INET,SOCK_STREAM,0);
    if(s==-1){
        printf("Could not create socket");
        fflush(stdout);
        fflush(stdin);
    }else{
        printf("Socket Successfully Created!!");
        fflush(stdout);
        fflush(stdin);
    }
    //init the socket
    server.sin_port=htons(8082);//set the port for communitcation
    server.sin_family=AF_INET;//use ipV4 
    server.sin_addr.s_addr=INADDR_ANY;
    //when INADDR_ANY is specified in the bind call, the socket will be bound to all local interfaces
    //bind
    if(bind(s,(struct sockaddr *)&server,sizeof(server))<0)
    {
        perror("Bind issue!!");
        return 1;
    }else{
        printf("Bind Complete !!");
    }
    fflush(stdout);
    fflush(stdin);
    //listen for connection
    listen(s,3);
    //Accept and incoming connection
    printf("Waiting for incoming connection from Client>>");
    fflush(stdout);
    connSize=sizeof(struct sockaddr_in);
    //accept connection from an incoming client
    while(cs=accept(s,(struct sockaddr *)&client,(socklen_t*)&connSize)){
        if(cs<0){
            perror("Can't establish connection");
            fflush(stdout);
            fflush(stdin);
            return 1;
        }else{
            printf("Connection from client accepted!!");
            fflush(stdout);
            fflush(stdin);
        }
        pthread_t sniffer_thread;
        new_socket =malloc(sizeof *new_socket);
        *new_socket=cs;
        printf("%d<-------",cs);
        if(pthread_create(&sniffer_thread,NULL,threadLogic,(void*)new_socket)<0){
            perror("could not create thread");
            return 1;
        }else{
            printf("thread created");
            fflush(stdout);
            fflush(stdin);
        }
    }
    return 0;
    
}
