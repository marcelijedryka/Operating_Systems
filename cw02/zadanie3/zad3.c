#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char** argv){

     if(argc != 2){
        printf("Please input correct arguments\n");
        return 0;
    }

    DIR* directory = opendir(argv[1]);
    long long size = 0;
    struct dirent *currentf;
    struct stat bufor_stat;

    while(currentf = readdir(directory)){

        stat(currentf->d_name, &bufor_stat);
        
        if(!S_ISDIR(bufor_stat.st_mode)){
            size = size + bufor_stat.st_size;
            printf("The size of file %s is %ld\n", currentf->d_name , bufor_stat.st_size);
        }

    }

    printf("Total size of pointed dictionary is %lld\n", size);

    closedir(directory);
    
    return 0;

}