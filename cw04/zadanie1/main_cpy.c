#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


void new_raise(){
    printf("Process %d is rising new signal\n" , getpid());
    raise(SIGUSR1);
}

void signal_handler(){
    printf("Process %d recieved new signal\n" , getpid());
}

int main(int argc , char** argv){

    if (argc != 2){
        printf("Invalid arguments\n");
        return 1;
    }

    char *command = argv[1];

    if(strcmp(command,"ignore") == 0){
        signal(SIGUSR1,SIG_IGN);
        new_raise();
    }

    if(strcmp(command,"mask") == 0){
        signal(SIGUSR1,SIG_IGN);
        new_raise();
    }
    
   if(strcmp(command,"pending") == 0){
        signal(SIGUSR1,SIG_IGN);
        new_raise();
        sigset_t set;
        sigpending(&set);
        if(sigismember(&set,SIGUSR1)){
            
            printf("Process : %d - Signal is pending\n" , getpid());
        }else{
            printf("Process : %d - Signal is not pending\n" , getpid());
        }
    }
    
    return 0;


}