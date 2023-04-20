#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

int req_counter = 0;
int request;
bool flag = true;
int sender_pid;

void handler(int signum , siginfo_t *si, void *p3 ) {

    printf("CATCHER >> New request has been recieved\n");
    sender_pid = si->si_pid;
    request = si->si_status;
    req_counter++;
    flag = false;
}

void count(){
    for (int i = 1; i <= 100; i++){
        printf("CATCHER >> %i\n", i);
    }
}

void get_current_time(){
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    printf("CATCHER >> Current time is: %s" , asctime (timeinfo));

}

void looper(){
    while(request == 4){
        get_current_time();
        sleep(1);
    }
}

int main(void){

    printf("Catcher is ready - PID : %d\n", getpid());

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1,&act,NULL);

    printf("All set\n");
   
    while(1){

        if(flag){continue;}

        if(request == 5 ){
            printf("CATCHER >> Catcher is closing\n");
            kill(sender_pid, SIGUSR1);
            exit(0);
            break;
        }

        if(request == 1){
            count();
            flag = true;
            kill(sender_pid, SIGUSR1);
        }
        
        if(request == 2){
            get_current_time();
            flag = true;
            kill(sender_pid, SIGUSR1);
        }

        if(request == 3){
            kill(sender_pid, SIGUSR1);
            printf("%d requests have been recieved since Catcher has started\n", req_counter);
            flag = true;
        }

        if(request == 4){
            kill(sender_pid, SIGUSR1);
            looper();
            flag = true;

        }
    }
    return 0;
}