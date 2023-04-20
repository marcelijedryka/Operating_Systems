#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>


bool flag = true;

void handler(int signum){
    printf("SENDER >> New Signal has been recieved\n");
    flag = false;
}


int main(int argc , char **argv){

    if (argc < 3){
        printf("Invalid amount of arguments\n");
        return 0;
    }

    signal(SIGUSR1,handler);

    int catcher_pid = atoi(argv[1]);

    for(int i = 2 ; i < argc ; i++){

        int req = atoi(argv[i]);
        if (req < 1 || req > 5) {
        fprintf(stderr, "SENDER >> Invalid request: %d\n", req);
        } 
        union sigval sig_val = {req};
        sigqueue(catcher_pid, SIGUSR1, sig_val);
        printf("SENDER >> New command %s has been sent\n", argv[i]);

        while(1){
            if(flag && (i <argc)){
                continue;
            }
            break;
        }

        flag = true;

    }

    return 0;
    
}

