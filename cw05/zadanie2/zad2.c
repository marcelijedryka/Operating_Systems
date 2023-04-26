#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <time.h>
#include<sys/wait.h>

double integral(double a , double b, double dx){

    double output = 0.0;
    a += dx/2.0;
    for(double x = a ; x < b ; x += dx){
        output+= (4/(x*x+1))*dx;
    }

    return output;
}


int main(int argc, char** argv){

    if(argc != 3){
        printf("Invalid number of arguments\n");
        return 1;
    }

    double dx = atof(argv[1]);
    int n = atoi(argv[2]);
    int fd[n][2];
    struct timespec start,end;
    double output;

    double step = 1.0/n;
    double a = 0.0;
    double b = 0.0;

    clock_gettime(CLOCK_REALTIME,&start);

    for(int i ; i < n ; i++){
        a = b;
        b += step;

        pipe(fd[i]);
        pid_t pid = fork();

        if(pid == 0){
            close(fd[i][0]);
            double part = integral(a,b,dx);
            write(fd[i][1], &part, sizeof(part));
            exit(0);
        }
    }

    while(wait(NULL)>0);

    for(int i ; i<n ; i++){
        close(fd[i][1]);
        double current;
        read(fd[i][0], &current, sizeof(current));
        output += current;
    }

    clock_gettime(CLOCK_REALTIME,&end);

    printf("The final value is %f Time - %fs" , output ,(end.tv_sec-start.tv_sec) + (end.tv_nsec-start.tv_nsec)/10e8);

    return 0;

}

