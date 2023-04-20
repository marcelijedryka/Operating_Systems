#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char **argv){

    if(argc != 2){
        printf("Input correct arguments");
        return 0;
    }

    int n = atoi(argv[1]);
    int pid;
    int current;
    int mother;


    for(int i = 0; i < n ; i++){

        pid = fork();

        if(pid == 0){
            current = getpid();
            mother = getppid();
            printf("Current process pid is: %d  Mother process pid is %d \n", current , mother);fflush(stdout);
            return 0;
        }
    }

    while(wait(0) > 0);
    printf("%d\n" , n);fflush(stdout);

    return 0;
}