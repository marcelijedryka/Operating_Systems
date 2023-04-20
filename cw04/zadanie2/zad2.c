#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int id = 0;
int depth = 0;
#define CALL_LIMIT 5

void handler(int signum , siginfo_t *si, void *p3 ){
        printf("Signal number: %d PID: %d UID: %d Status: %d POSIX ID: %d\n", si->si_signo, si->si_pid , si->si_uid , si-> si_status , si->si_timerid);

}

void depth_handler(int signum , siginfo_t *si, void *p3 ){
    int current = id;
    printf("IN -> ID : %d , DEPTH: %d\n" , current , depth);
    
    id++;
    depth++;

    if(id < CALL_LIMIT){
        raise(SIGUSR1);
    }

    depth--;

    printf("OUT ->  ID : %d , DEPTH: %d\n" , current , depth);
}

void make_action(void (*func)(int, siginfo_t*, void*), int sigum , int flag){
    struct sigaction act;
    act.sa_sigaction=func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = flag;
    sigaction(sigum,&act,NULL);

}



int main(void){

    make_action(handler , SIGUSR1 , SA_SIGINFO);
    printf("SA_SIGINFO TEST\n");
    raise(SIGUSR1);

    make_action(depth_handler , SIGUSR1 , SA_NODEFER);
    printf("SA_NODEFER TEST\n");
    raise(SIGUSR1);

    make_action(depth_handler , SIGUSR1 , SA_RESETHAND);
    printf("SA_RESETHAND TEST\n");
    raise(SIGUSR1);
    
    return 0;

}

