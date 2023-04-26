#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <time.h>
#include <signal.h>

#define MAX_SIZE 256


typedef struct CommandBuff {
    long command;
    key_t queue_Key;
    int client_ID;
    int reciever_ID;
    char message[MAX_SIZE];
    struct tm time_struct;
} CommandBuff;


// LIST - 1
// 2ALL - 2
// 2ONE - 3
// STOP - 4
// INIT - 5
