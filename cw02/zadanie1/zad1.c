#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include<fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>


void switch_char_lib(int seek , int swap, char *filename, char *newfile){

    size_t read_size;
    size_t write_size;
    char *buffer = calloc(1024, sizeof(char));
    FILE *oldf = fopen(filename, "r");
    FILE *newf = fopen(newfile, "w");
    int counter = 0;
    read_size = fread(buffer,sizeof(char),1024,oldf);

    while(1){

        for(int i = 0; i<1024; i++){
            if(buffer[i] == seek){
                buffer[i] = swap;
                counter++;
            }
        }

        write_size = fwrite(buffer,sizeof(char),read_size,newf);

        if(write_size < 1024){
            break;
        }else{
            read_size = fread(buffer,sizeof(char),1024,oldf);
        }
    }

    fclose(oldf);
    fclose(newf);
    free(buffer);

    printf("New file %s has been created with stdlib method - %d chars have been replaced\n", newfile , counter);


}


void switch_char_sys(int seek , int swap, char *filename, char *newfile){

    char *buffer = calloc(1024, sizeof(char));
    int oldf = open(filename,O_RDONLY);
    int newf = open(newfile, O_WRONLY | O_CREAT);
    int counter = 0;
    int write_size;
    int read_size = read(oldf,buffer,sizeof(char)*1024);

    while(1){

        for(int i = 0; i<1024; i++){
            if(buffer[i] == seek){
                buffer[i] = swap;
                counter++;
            }
        }

        write_size = write(newf,buffer,read_size);

        if(write_size < 1024){
            break;
        }else{
            read_size = read(oldf,buffer,sizeof(char)*1024);
        }
    }

    close(oldf);
    close(newf);
    free(buffer);

    printf("New file %s has been created with system functions- %d chars have been replaced\n", newfile , counter);

}


int main(int argc, char** argv){

    if(argc != 5){
        printf("Please input correct arguments\n");
        return 0;
    }
    int seek = atoi(argv[1]);
    int swap = atoi(argv[2]);
    char *filename = argv[3];
    char *newfile = argv[4];

    struct timespec real_start, real_end;
    struct tms tms_start , tms_end;

    clock_gettime(CLOCK_REALTIME,&real_start);

    switch_char_lib(seek,swap,filename, newfile);

    clock_gettime(CLOCK_REALTIME,&real_end);

    double divider = 1000000000;
    long double realt = (long double) (real_end.tv_sec - real_start.tv_sec) + (real_end.tv_nsec - real_start.tv_nsec) / divider;
    
    printf("Execution time (std library methods): %Lfs\n", realt);

    clock_gettime(CLOCK_REALTIME,&real_start);

    switch_char_sys(seek,swap,filename, "sys_result.txt");

    clock_gettime(CLOCK_REALTIME,&real_end);

    realt = (long double) (real_end.tv_sec - real_start.tv_sec) + (real_end.tv_nsec - real_start.tv_nsec) / divider;
    
    printf("Execution time (system functions): %Lfs\n", realt);

    return 0;
}