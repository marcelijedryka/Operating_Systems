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
        return 0;
    }

    char *command = argv[1];
    pid_t pid;

    if(strcmp(command,"ignore") == 0){
        signal(SIGUSR1,SIG_IGN);

        pid = fork();
        if (!pid){
             new_raise();
             return 0;
        }
        execl("./main_cpy", "./main_cpy", command, NULL);
    }
    
    if(strcmp(command,"handler") == 0){
        signal(SIGUSR1, signal_handler);
        new_raise();

        pid = fork();

        if(!pid){
            new_raise();
            return 0;
        }
    }

    if(strcmp(command,"mask") == 0 || strcmp(command,"pending") == 0){
        sigset_t blocked;
        sigemptyset(&blocked);
        sigaddset(&blocked, SIGUSR1);
        sigprocmask(SIG_BLOCK, &blocked, NULL);

        if(strcmp(command,"mask") == 0){
            pid = fork();
            if (!pid){
                new_raise();
                return 0;
            }
            execl("./main_cpy", "./main_cpy", command, NULL);
        }else{
            sigset_t set;
            pid = fork();
            if (!pid){
                sigpending(&set);
                if(sigismember(&set,SIGUSR1)){
                    printf("Process : %d - Signal is pending\n" , getpid());
                }else{
                    printf("Process : %d - Signal is not pending\n" , getpid());

                }
                return 0;
            }
            execl("./main_cpy", "./main_cpy", command, NULL);

        }
    }
    return 0;
}

