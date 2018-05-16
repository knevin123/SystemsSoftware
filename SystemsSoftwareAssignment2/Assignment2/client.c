#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
    //init client variable
    int SID;
    struct sockaddr_in server;
    char clientMessage[500];
    char username[500];
    char password[500];
    char serverMessage[500];
    char transferMessage[500];
    int login=0;
    int choice=0;
    char filepath[200];
    char trans[50];
    char sendpath[200];
    char filename[200];
    char file[200];
    char option[2];
    //create socket
    SID=socket(AF_INET,SOCK_STREAM,0);
    if(SID==-1){
        printf("Could not create socket");
    }else{
        printf("Socket Successfully Created!!");
    }
    fflush(stdout);
    //init the socket
    server.sin_port=htons(8082);//set the port for communitcation
    server.sin_family=AF_INET;//use ipV4 
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    //when INADDR_ANY is specified in the bind call, the socket will be bound to all local interfaces
    //connect to server
    if(connect(SID,(struct sockaddr *)&server,sizeof(server))<0){
        printf("connect failed. Error");
        return 1;
    }
    printf("Connected to server ok!!\n");
    fflush(stdout);
    fflush(stdout);
    send(SID,"login",strlen("login"),0);

	//enter user data
    printf("\nEnter username : ");
    fflush(stdout);
    memset(username,0,500);
    scanf("%s",username);
    printf("\nEnter password : ");
    scanf("%s",password);
    fflush(stdout);
    fflush(stdin);
	//concatonate user data before sending
    strcat(username,",");
    strcat(username,password);
    strcat(username,"\n");
    
	//send user data
    if(send(SID,username,strlen(username),0)<0){
        printf("Send failed");
        fflush(stdout);
        fflush(stdin);
        return 1;
    }
    fflush(stdout);
    //recieve reply on whether user is sucessful
	//or not
    if(recv(SID,serverMessage,500,0)<0){
        printf("IO error");
        fflush(stdout);
        fflush(stdin);
    }
    fflush(stdout);
	//check for login success or failure
    if(strcmp("200",serverMessage)==0){
        login=1;
        printf("\nuser success");
    }else{
        printf("\nuser failed");
        fflush(stdout);
        fflush(stdin);
        exit(0);
    }
    fflush(stdout);
    while(1){
        fflush(stdout);
		//chose to transfer file
        printf("\n1:Tranfer File\n");
        printf("2:Logout\n");
        printf("Make Selection:\n");
        fflush(stdout);
        fflush(stdin);
        scanf("%d",&choice);
        if(choice==1){
			//ask for file name and send message that transfer is starting
            send(SID,"transfer",strlen("transfer"),0);
            printf("Type in file name : ");
            fflush(stdout);
            fflush(stdin);
            memset(filepath,0,200);
            scanf("%s",filepath);
            strcpy(file,filepath);
			//get the file name from the file path the user enters
			//split the path using /
            const char s[2]="/";
            char *token;
            //get first token
            token=strtok(filepath,s);
            //walk through other tokens, last one will be file name
            while(token!=NULL){
                fflush(stdout);
                fflush(stdin);
                strcpy(filename,token);
                token=strtok(NULL,s);
            }
            //pick a destination folder
            printf("Choose a destination folder:\n1:Root\n2:Sales\n3:Promotions\n4:Offers\n5:Marketing \n");
            scanf("%s",option);
			//see what option they choose
			//add the target folder to the filename
            if(strcmp("1",option)==0){
                memset(sendpath,0,200);
                strcpy(sendpath,filename);
            }else if(strcmp("2",option)==0){
                memset(sendpath,0,200);
                strcat(sendpath,"Sales");
                strcat(sendpath,"/");
                strcat(sendpath,filename);
            }else if(strcmp("3",option)==0){
                memset(sendpath,0,200);
                strcat(sendpath,"Promotions");
                strcat(sendpath,"/");
                strcat(sendpath,filename);
            }else if(strcmp("4",option)==0){
                memset(sendpath,0,200);
                strcat(sendpath,"Offers");
                strcat(sendpath,"/");
                strcat(sendpath,filename);
            }else if(strcmp("5",option)==0){
                memset(sendpath,0,200);
                strcat(sendpath,"Marketing");
                strcat(sendpath,"/");
                strcat(sendpath,filename);
            }
			//send the filename to the server
            send(SID,sendpath,strlen(sendpath),0);
            fflush(stdout);
            fflush(stdin);    
			//wait for response from server for confirmation they got filename
            recv(SID,trans,200,0);
			
            printf("\n%s\n",trans);
            char file_buffer[512];
			//sending file to the server
            printf("[client] Sending %s to the server ...\n",file);
            fflush(stdout);
            fflush(stdin);
			//open file
            FILE *file_open=fopen(file,"r");
            bzero(file_buffer,512);
            int block_size,i=0;
			//read in file
            while((block_size=fread(file_buffer,sizeof(char),512,file_open))>0){
				//show progress
                printf("Data Sent %d = %d\n",i,block_size);
                fflush(stdout);
                fflush(stdin);
				//send data
                if(send(SID,file_buffer,block_size,0)<0){
                    exit(1);
                }
                bzero(file_buffer,512);
                i++;
            }
            fflush(stdout);
            fflush(stdin);
            fclose(file_open);
			//waiting for sucess message
            memset(transferMessage,0,500);
            recv(SID,transferMessage,500,0);
			//check if sucess or failSending file to the server
            if(strcmp(transferMessage,"Done")==0){
                printf("Transfer Complete\n");
                fflush(stdout);
                fflush(stdin);
            }else{
                printf("Transfer Failed\n");
                fflush(stdout);
                fflush(stdin);
            }
        }else if (choice==2){
            exit(1);
        }else{ 
            printf("Incorrect selection");
        }
        fflush(stdout);
    }
    close(SID);
    return 0;
}