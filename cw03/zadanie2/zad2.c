#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char **argv){

    if( argc != 2){
        printf("Input correct arguments\n");
        return 0;
    }

    printf("%s" ,argv[0]);

    char *path = argv[1];

    execl("/bin/ls" ,"ls", path, NULL);
    

    return 0;

}