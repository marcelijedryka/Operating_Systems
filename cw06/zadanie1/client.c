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

#define MY_SERVER_ID 1
#define PATH getenv("HOME")

key_t queueKey;
int queueID;
int serverID;
int clientID;

int init(){

    time_t current_time = time(NULL);

    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    mybuff->time_struct = *localtime(&current_time);
    mybuff->command = 5;
    mybuff->queue_Key = queueKey;
    msgsnd(serverID, mybuff, sizeof(CommandBuff),0);
    msgrcv(queueID, mybuff, sizeof(CommandBuff),0,0);

    int current_id = mybuff->client_ID;

    printf("My client ID is %d\n" , current_id);

    return current_id;

}

void command_list(){

    time_t current_time = time(NULL);

    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    mybuff->time_struct = *localtime(&current_time);
    mybuff->client_ID = clientID;
    mybuff->command = 1;

    msgsnd(serverID, mybuff, sizeof(CommandBuff),0);
    msgrcv(queueID, mybuff, sizeof(CommandBuff),0,0);
}

void command_2all(char* message){

    time_t current_time = time(NULL);

    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    mybuff->time_struct = *localtime(&current_time);
    mybuff->client_ID = clientID;
    mybuff->command = 2;
    strcpy(mybuff->message, message);

    msgsnd(serverID, mybuff, sizeof(CommandBuff),0);
}

void command_2one(char* message , int reciever){

    time_t current_time = time(NULL);

    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    mybuff->time_struct = *localtime(&current_time);
    mybuff->client_ID = clientID;
    mybuff->reciever_ID = reciever;
    mybuff->command = 3;
    strcpy(mybuff->message, message);
    msgsnd(serverID, mybuff, sizeof(CommandBuff),0);
}

void command_stop(){

    time_t current_time = time(NULL);

    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    mybuff->time_struct = *localtime(&current_time);
    mybuff->client_ID = clientID;
    mybuff->command = 4;

    msgsnd(serverID, mybuff, sizeof(CommandBuff),0);
    msgctl(queueID, IPC_RMID, NULL);

    exit(0);

}

void message_handler(){

    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    if(msgrcv(queueID,mybuff, sizeof(CommandBuff),0,IPC_NOWAIT) >= 0 ){
        if(mybuff->command == 4){
        printf("STOP command has been recieved , exiting\n");
        command_stop();
        }else{
            struct tm tmp_time = mybuff -> time_struct;
            printf("Client %d has sent a message at %02d:%02d:%02d \n", mybuff->client_ID,tmp_time.tm_hour,tmp_time.tm_min,tmp_time.tm_sec);
            printf("Message : %s\n" , mybuff->message);
        }
    }
}




int main(void){

    queueKey = ftok(PATH , getpid());
    queueID = msgget(queueKey, IPC_CREAT | 0666);
    key_t serverKey = ftok(PATH, MY_SERVER_ID);
    serverID = msgget(serverKey, 0600);


    clientID = init();
    size_t len =0;
    ssize_t command_size;
    char* command = NULL;

    signal(SIGINT, command_stop);

    while(1){

        printf("TYPE COMMAND > ");
        command_size = getline(&command, &len, stdin);
        command[command_size-1] ='\0';

        message_handler();

        char *current = strtok(command," ");

        if (strcmp(current, "") == 0) {
            continue;
        }
         printf("%s\n" , current);

        if(strcmp(current , "LIST") == 0){
            command_list();
        }
        else if(strcmp(current , "STOP") == 0){
            command_stop();
        }
        else if(strcmp(current , "2ONE") == 0){
            current = strtok(NULL, " ");
            int reciever = atoi(current);
            printf("%d\n" , reciever);
            current = strtok(NULL, " ");

            command_2one(current , reciever);
        
        }
        else if (strcmp(current , "2ALL") == 0){
            current = strtok(NULL, " ");
            command_2all(current);
        }
        else{
            printf("Command has not been recognized\n");
        }


    }

    return 0;
}







