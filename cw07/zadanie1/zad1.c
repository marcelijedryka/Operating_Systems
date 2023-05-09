#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <sys/types.h>

#define MAX_SIZE 1024

typedef struct {
    key_t queueKey;
    key_t barberKey;
    key_t chairKey;
    key_t openSpotKey;
    key_t freeBarberKey;
    key_t freeChairKey;
    key_t firstInQueueKey;
} keySet;

typedef struct {
    int queueID;
    int barberID;
    int chairID;
    int openSpotID;
    int freeBarberID;
    int freeChairID;
    int firstInQueueID;
} IDSet;

int m,n,p,f;
keySet keys;
IDSet IDs;

int client_counter = 0;

void create_sem(int id, int n){

    for (int i = 0 ; i < n ; i++){
        if(semctl(id,i,SETVAL,1) == -1){
            perror("semctl failed");
        }
    }
}

void set_value(int id, int value){
    int *shmaddr = shmat(id,NULL,0);
    *shmaddr = value;
    if (shmdt(shmaddr) == -1) {
        perror("shmdt failed");
    }
}

int get_value(int id){
    int *shmaddr = shmat(id,NULL,0);
    int value = *shmaddr;
    if (shmdt(shmaddr) == -1) {
        perror("shmdt failed");
    }
    return value;
}

void initialize(char *cwd){

    keys.queueKey = ftok(cwd,0);
    keys.barberKey = ftok(cwd,7);
    keys.chairKey = ftok(cwd,2);
    keys.openSpotKey = ftok(cwd,3);
    keys.freeBarberKey = ftok(cwd,4);
    keys.freeChairKey = ftok(cwd,5);
    keys.firstInQueueKey = ftok(cwd,6);

    IDs.queueID = semget(keys.queueKey,p,IPC_CREAT|0600);
    IDs.barberID = semget(keys.barberKey,m,IPC_CREAT|0600);
    IDs.chairID = semget(keys.chairKey,n,IPC_CREAT|0600);
    IDs.openSpotID = shmget(keys.openSpotKey,sizeof(int),IPC_CREAT|0600);
    IDs.freeBarberID = shmget(keys.freeBarberKey,sizeof(int),IPC_CREAT|0600);
    IDs.freeChairID= shmget(keys.freeChairKey,sizeof(int),IPC_CREAT|0600);
    IDs.firstInQueueID= shmget(keys.firstInQueueKey,sizeof(int),IPC_CREAT|0600);

    create_sem(IDs.queueID, p);
    create_sem(IDs.barberID, m);
    create_sem(IDs.chairID, n);
    set_value(IDs.openSpotID, p);
    set_value(IDs.freeBarberID, m);
    set_value(IDs.freeChairID, n);
    set_value(IDs.firstInQueueID, 0);

    printf("All initialized\n");

}

void clear(){

    while(wait(0) > 0);

    semctl(IDs.queueID,0,IPC_RMID);
    semctl(IDs.barberID,0,IPC_RMID);
    semctl(IDs.chairID,0,IPC_RMID);

    shmctl(IDs.openSpotID,IPC_RMID,NULL);
    shmctl(IDs.freeBarberID,IPC_RMID,NULL);
    shmctl(IDs.freeChairID,IPC_RMID,NULL);
    shmctl(IDs.firstInQueueID,IPC_RMID,NULL);

    printf("All cleared\n");

}

int pick_barber(){
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_op = -1;
    int value;

    for(int i = 0 ; i < m ; i++){
        value = semctl(IDs.barberID ,i ,GETVAL, NULL);
        if(value==1){
            set_value(IDs.freeBarberID , get_value(IDs.freeBarberID)-1);
            buf.sem_num = i;
            semop(IDs.barberID, &buf, 1);
            return i;
        }
    }

    return -1;

}

int pick_chair(){
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_op = -1;
    int value;

    for(int i = 0 ; i < n ; i++){
        value = semctl(IDs.chairID ,i ,GETVAL, NULL);
        if(value==1){
            set_value(IDs.freeChairID , get_value(IDs.freeChairID)-1);
            buf.sem_num = i;
            semop(IDs.chairID, &buf, 1);
            return i;
        }
    }

    return -1;

}

void free_slot(int id , int freeID , int objectID){
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_num = objectID;
    buf.sem_op = 1;

    semop(id, &buf ,1);
    set_value(freeID , get_value(freeID)+1);

}

void get_haircut(int clientID){
    srand(time(NULL));

    int barber = pick_barber();
    int chair = pick_chair();

    printf("Client %d is now getting a haircut from barber %d on chair number %d\n",clientID,barber,chair);

    int haircut_time = (rand() % f) + 1;
    sleep(haircut_time);

    free_slot(IDs.barberID , IDs.freeBarberID , barber);
    free_slot(IDs.chairID , IDs.freeChairID , chair);

    printf("Service for Client %d has ended\n" , clientID);
    
}

int add_to_queue(){
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_op = -1;
    int value;

    for(int i = get_value(IDs.firstInQueueID) ; i < p ; i++){
        value = semctl(IDs.queueID ,i ,GETVAL, NULL);
        if(value==1){
            set_value(IDs.openSpotID , get_value(IDs.openSpotID)-1);
            buf.sem_num = i;
            semop(IDs.queueID, &buf, 1);
            return i;
        }
    }

    for(int i = 0 ; i < get_value(IDs.firstInQueueID) ; i++){
        value = semctl(IDs.queueID ,i ,GETVAL, NULL);
        if(value==1){
            set_value(IDs.openSpotID , get_value(IDs.openSpotID)-1);
            buf.sem_num = i;
            semop(IDs.queueID, &buf, 1);
            return i;
        }
    }

    return -1;

}

void pick_next(){
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_num = get_value(IDs.firstInQueueID);
    buf.sem_op = 1;

    semop(IDs.queueID, &buf, 1);
    set_value(IDs.openSpotID , get_value(IDs.openSpotID)+1);
    set_value(IDs.firstInQueueID , (get_value(IDs.firstInQueueID)+1)%p);
}


void new_client(){
    int clientID = client_counter;

    printf("New Client in the shop - ClientID : %d\n" , clientID);

    if(get_value(IDs.openSpotID) == 0){
        printf("Service is not available for Client %d\n" , clientID);
        return;
    }
    else if(get_value(IDs.freeBarberID) == 0 || get_value(IDs.freeChairID) == 0 || get_value(IDs.openSpotID) < p){
        printf("Client %d is now waiting in queue\n" , clientID);
        int place = add_to_queue();
        while(semctl(IDs.queueID,place, GETVAL ,NULL)==0){}
        get_haircut(clientID);

        if(get_value(IDs.openSpotID) < p){
            pick_next();
        }
        return;
    }
    else{
        get_haircut(clientID);
        if(get_value(IDs.openSpotID) < p){
            pick_next();
        }
        return;
    }
    printf("Service is not available for Client %d\n" , clientID);
}


int main(int argc, char **argv){


    if(argc != 5){
        printf("Please input right amount of arguments\n");
        return 1;
    }

    m = atoi(argv[1]);
    n = atoi(argv[2]);
    p = atoi(argv[3]);
    f = atoi(argv[4]);

    if(m <= 0 || n<= 0 || p <= 0){
        printf("All arguments must be grater than 0\n");
        return 1;
    }

    char cwd[MAX_SIZE];
    getcwd(cwd , sizeof(cwd));
    initialize(cwd);

    char input[MAX_SIZE];
    int pid;

    printf("Press 1 to add a new client, press 2 to end the program\n");

    while(1){

        fgets(input,MAX_SIZE,stdin);

        if(atoi(input) == 2){
            break;
        }
        else if(atoi(input) == 1){
            client_counter++;
            pid = fork();

            if(pid == 0){
                new_client();
                return 0;
            }
            
        }
        else{
            printf("Invalid value has been inserted - try again\n");
        }
    }

    clear();

    return 0;
}