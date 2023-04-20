#include <stdio.h>
#include <sys/stat.h>
#include <ftw.h>

long long size = 0;

int search_dir(const char *path ,const struct stat *filler, int filetype){
    if(filetype != FTW_D){
        struct stat bufor_stat;
        stat(path,&bufor_stat);
        size = size + bufor_stat.st_size;
        printf("The size of file %s is %ld\n", path , bufor_stat.st_size);
    }

    return 0;
}


int main(int argc, char** argv){

     if(argc != 2){
        printf("Please input correct folder path\n");
        return 0;
    }

    char *path = argv[1];
    
    int output = ftw(path , search_dir ,1);

    if(output != 0){
        printf("Something went wrong, try again");
    }

    printf("Total size: %lld\n" , size);

    return 0;

}