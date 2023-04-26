#include<unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h> 
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "cmdbuff.h"
#include <sys/types.h>
#include <errno.h>


#define MY_SERVER_ID 1
#define PATH getenv("HOME")
#define CLIENT_LIMIT 5


int new_client_ID = 0;
int server_queue;
key_t client_queue[CLIENT_LIMIT];

void handle_command(CommandBuff *mybuff){

    struct tm tmp_time = mybuff->time_struct;

    FILE *file = fopen("command_history.txt", "a");

    if(mybuff->command == 0){
        if(mybuff->client_ID == -1){
            printf("Client limit has been reached\n");
        }else{
            printf("Server : New client has been initialized - Client ID: %d\n" ,mybuff->client_ID );
            fprintf(file, "Server : New client has been initialized - Client ID: %d\n", mybuff->client_ID);
        }
    }else if(mybuff->command == 1){
        fprintf(file, "Server : LIST command - Client ID: %d\n", mybuff->client_ID);
    }else if(mybuff->command == 2){
        fprintf(file, "Server : 2ALL command - Client ID: %d\n", mybuff->client_ID);
        fprintf(file, "Message: %s\n", mybuff->message);
    }else if(mybuff->command == 3){
        fprintf(file, "Server : 2ONE command - Client ID: %d RECIEVER ID: %d\n", mybuff->client_ID , mybuff->reciever_ID);
        fprintf(file, "Message: %s\n", mybuff->message);
    }else{
        fprintf(file, "Server : STOP command - Client ID: %d\n", mybuff->client_ID);

    }

    fprintf(file, "sent at: %02d:%02d:%02d\n\n",tmp_time.tm_hour,tmp_time.tm_min,tmp_time.tm_sec);

    fclose(file);

}


void close_server(){

    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    for (int i = 0 ; i < CLIENT_LIMIT; i++){
        if (client_queue[i] != -1){
            mybuff->command = 4;
            int client_queueID = msgget(client_queue[i],0);
            msgsnd(client_queueID, mybuff,sizeof(CommandBuff),0);
            msgrcv(server_queue, mybuff,sizeof(CommandBuff),5,0);
        }

    }

    msgctl(server_queue, IPC_RMID, NULL);

    exit(0);

}

void init(CommandBuff *mybuff){
    while(client_queue[new_client_ID] != -1 && (new_client_ID < CLIENT_LIMIT - 1)){
        new_client_ID++;
    }

    if(client_queue[new_client_ID] != -1 && (new_client_ID < CLIENT_LIMIT - 1)){
        mybuff->client_ID = -1;
    }
    else{
        mybuff->client_ID = new_client_ID;
        client_queue[new_client_ID] = mybuff->queue_Key;
    }

    int client_queueID = msgget(mybuff->queue_Key,0600);
    msgsnd(client_queueID, mybuff, sizeof(CommandBuff),0);

    handle_command(mybuff);
}

void list(CommandBuff *mybuff){

    for (int i = 0; i < CLIENT_LIMIT; i++){
            if(client_queue[i]!=-1){
                printf("Client id: %d\n", i);
            }
        }
    int client_queueID = msgget(mybuff->queue_Key,0600);
    msgsnd(client_queueID, mybuff, sizeof (CommandBuff), 0);
    handle_command(mybuff);

}

void toall(CommandBuff *mybuff){
    for(int i =0 ; i < CLIENT_LIMIT ; i++){
        if(client_queue[i] != -1 && mybuff->client_ID != i){
            int recieverqueueID = msgget(client_queue[i],0);
            msgsnd(recieverqueueID,mybuff,sizeof(CommandBuff),0);
        }
    }
    handle_command(mybuff);
}

void toone(CommandBuff *mybuff){
    int recieverqueueID = msgget(client_queue[mybuff->reciever_ID],0);
    msgsnd(recieverqueueID,mybuff,sizeof(CommandBuff),0);
    handle_command(mybuff);
}

void stop(CommandBuff *mybuff){
    client_queue[mybuff->client_ID] = -1;

    if(mybuff->client_ID < new_client_ID){
        new_client_ID = mybuff->client_ID;
    }
    handle_command(mybuff);
}




int main(){

    printf("Server starting...\n");

    for (int i = 0 ; i < CLIENT_LIMIT; i++){
        client_queue[i]=-1;
    }
    key_t queueKey = ftok(PATH, MY_SERVER_ID);
    server_queue = msgget(queueKey, IPC_CREAT | 0666);
    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    signal(SIGINT, close_server);

    while(1){
        
        msgrcv(server_queue, mybuff, sizeof(CommandBuff), -6,0);
      
        if(mybuff->command == 5){
            printf("Init command has been recieved\n");
            init(mybuff);
        }
        if(mybuff->command == 1){
            list(mybuff);
        }
        if(mybuff->command == 2){
            toall(mybuff);
        }
        if(mybuff->command == 3){
            toone(mybuff);
        }
        if(mybuff->command == 4){
            stop(mybuff);
        }

    }

    return 0;

}