#include "mylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include <dlfcn.h>
#define BUFFERSIZE 20

int main(){

    char *command = (char*)calloc(BUFFERSIZE,sizeof(char));
    char *value = (char*)calloc(BUFFERSIZE,sizeof(char));
    char *line = (char*)calloc(BUFFERSIZE,sizeof(char));
    mystruct *s;

    struct timespec real_start, real_end;
    struct tms tms_start , tms_end;

    printf("Please run command INIT SIZE to create structure\n"
            "Then u can use commands listed below:\n"
            "COUNT FILE\n"
            "SHOW INDEX\n"
            "DELETE INDEX\n"
            "DESTROY\n"
            "To exit type EXIT\n");


    void *grip=dlopen("./libmylibshared.so", RTLD_LAZY);

    mystruct * (*init_size)(int size);
    init_size = dlsym(grip,"init_size");

    void (*count_file)(mystruct *s, char *filename);
    count_file = dlsym(grip,"count_file");

    char* (*show_index)(mystruct * s , int index);
    show_index = dlsym(grip,"show_index");

    void (*delete_index)(mystruct * s, int index);
    delete_index = dlsym(grip,"delete_index");

    void (*destroy)(mystruct * s);
    destroy = dlsym(grip,"destroy");

    

    while(fgets(line, BUFFERSIZE,stdin)){
        clock_gettime(CLOCK_REALTIME,&real_start);
        times(&tms_start);
        if(line[strlen(line)-1]=='\n'){
            line[strlen(line)-1]='\0';
        }
        command = strtok(line, " ");
        if(strcmp(line,"EXIT") == 0){
            break;
        }
      
        if(strcmp(line, "DESTROY") == 0){
            destroy(s);
        }else{
            value = strtok(NULL, " ");

            if(strcmp(command, "COUNT") == 0){
                count_file(s,value);
            
            }
            else{
                int val = atoi(value);

                if(strcmp(command, "INIT") == 0){
                    s = init_size(val);
                
                }

                if(strcmp(command, "SHOW") == 0){
                    char * id = show_index(s,val);
                
                }

                if(strcmp(command, "DELETE") == 0){
                    delete_index(s,val);
                
                }

            }
        }

            clock_gettime(CLOCK_REALTIME,&real_end);
            times(&tms_end);
            double divider = 1000000000;
            long double realt = (long double) (real_end.tv_sec - real_start.tv_sec) + (real_end.tv_nsec - real_start.tv_nsec) / divider;
            long double usert = (long double) (tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK);
            long double systemt = (long double) (tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK);
            printf("Real Time: %Lfs ",realt);
            printf("User Time: %Lfs ",usert);
            printf("System Time: %Lfs\n",systemt);
    }

    free(line);

    return 0;


}