#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <string.h>

void reverse_file_by_block(char *filename, char *reversed){

    FILE* oldf = fopen(filename, "r");
    FILE* newf = fopen(reversed, "w");
    char *buffer = calloc(1024, sizeof(char));
    size_t read;
    int i = 0;
    int j;
    int end;
    char tmp;

    fseek(oldf,0,SEEK_END);
    end = ftell(oldf);

    size_t bnumber = end / ((size_t) 1024);
 
    while(i<=bnumber){

        fseek(oldf, i*(-1024), SEEK_END); 
        read = fread(buffer,sizeof(char),1024,oldf);
        buffer[read] = 0;
        j = 0;

        while(j < read/2){
            tmp = buffer[j];
            buffer[j] = buffer[read-(j+1)];
            buffer[read-(j+1)] = tmp;
            j++;
            
        }
        i++;

        fwrite(buffer,sizeof(char),read,newf);
    }

    fclose(oldf);
    fclose(newf);
    free(buffer);
    

}


void reverse_file_char_by_char(char *filename, char *reversed){

    FILE* oldf = fopen(filename, "r");
    FILE* newf = fopen(reversed, "w");
    int i = 0;
    int end;

    fseek(oldf,0,SEEK_END);
    end = ftell(oldf);

    while(i < end){
        i++;
        fseek(oldf,-i,SEEK_END);
        fputc(fgetc(oldf),newf);

    }

    printf("New file %s has been created\n",reversed);

    fclose(oldf);
    fclose(newf);

}



int main(int argc, char** argv){

    if(argc != 3){
        printf("Please input correct arguments\n");
        return 0;
    }

    char *filename = argv[1];
    char *newfile = argv[2];
    struct timespec real_start, real_end;
    struct tms tms_start , tms_end;

    clock_gettime(CLOCK_REALTIME,&real_start);
    reverse_file_char_by_char(filename, newfile);
    clock_gettime(CLOCK_REALTIME,&real_end);
    double divider = 1000000000;
    long double realt = (long double) (real_end.tv_sec - real_start.tv_sec) + (real_end.tv_nsec - real_start.tv_nsec) / divider;

    printf("Execution time of char by char function is: %Lfs\n", realt);

    clock_gettime(CLOCK_REALTIME,&real_start);
    reverse_file_by_block(newfile,"twice_reversed.txt");
    clock_gettime(CLOCK_REALTIME,&real_end);
    realt = (long double) (real_end.tv_sec - real_start.tv_sec) + (real_end.tv_nsec - real_start.tv_nsec) / divider;

    printf("Execution time of block by block function is: %Lfs\n", realt);

    return 0;
}