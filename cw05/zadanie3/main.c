#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <time.h>
#include<sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_PATH "/tmp/fifo_queue"
#define BUFF_SIZE 256


int main(int argc, char** argv){

    if(argc != 3){
        printf("Invalid number of arguments\n");
        return 1;
    }

    int n = atoi(argv[2]);
    struct timespec start,end;
    double output;

    double step = 1.0/n;
    double a = 0.0;
    double b = 0.0;
    char x1[BUFF_SIZE];
    char x2[BUFF_SIZE];

    clock_gettime(CLOCK_REALTIME,&start);


    mkfifo(FIFO_PATH, 0666);

    for(int i=0 ; i < n; i++){
        a = b;
        b += step;
        
        pid_t pid = fork();

        if(pid == 0){

            snprintf(x1,BUFF_SIZE,"%f",a);
            snprintf(x2,BUFF_SIZE,"%f",b);
            execl("./int","int",x1,x2,argv[1],NULL);

        }else{
            char buffer[BUFF_SIZE];
            double part;
            int myfifo = open(FIFO_PATH , O_RDONLY);

            read(myfifo,buffer,sizeof(buffer));
            sscanf(buffer,"%lf",&part);

            output+= part;

            close(myfifo);

        }
    }

    clock_gettime(CLOCK_REALTIME,&end);

    printf("The final value is %f Time - %fs" , output ,(end.tv_sec-start.tv_sec) + (end.tv_nsec-start.tv_nsec)/10e8);

    return 0;

}