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

double integral(double a , double b, double dx){

    double output = 0.0;
    a += dx/2.0;
    for(double x = a ; x < b ; x += dx){
        output+= (4/(x*x+1))*dx;
    }

    return output;
}


int main(int argc, char** argv){
   
    double a = atof(argv[1]);
    double b =atof(argv[2]);
    double dx = atof(argv[3]);
    char buffer[BUFF_SIZE];


    double part = integral(a,b,dx);

    int myfifo = open(FIFO_PATH , O_WRONLY);
    size_t size = snprintf(buffer,BUFF_SIZE,"%f\n",part);
    write(myfifo ,buffer , size);

    close(myfifo);

    return 0;
}

