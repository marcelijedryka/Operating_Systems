#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include<sys/stat.h>
#include<string.h>
#include <sys/wait.h>
#include<limits.h>

#define MAX_SIZE 255

void check_directory(char *path , char *pattern){

  DIR *directory;
  struct dirent *dir_entry;
  struct stat bufor_stat;
  pid_t pid;


  if(stat(path,&bufor_stat) == -1){
    perror("Stat error\n");
    return;
  }

  if(S_ISDIR(bufor_stat.st_mode)){


    directory = opendir(path);

    if(!directory){
    perror("Wrong directiory\n");
      return;
    }

    while((dir_entry = readdir(directory)) != NULL){

      if(!(strcmp(dir_entry->d_name , ".") == 0 || strcmp(dir_entry->d_name , "..") == 0)){


        char *new_path = calloc(PATH_MAX , sizeof(char));

        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, dir_entry->d_name);

        pid = fork();

        if(pid == 0){
          check_directory(new_path,pattern);
          return;
        }
        free(new_path);
      }
    }

    closedir(directory);
      
  }else{
    char *buffer = calloc(MAX_SIZE , sizeof(char));
    FILE *f = fopen(path , "r");
    fread(buffer , sizeof(char) , strlen(pattern) ,f);

    if(strcmp(buffer,pattern) == 0){
        printf("Path : %s Process ID : %d\n", path, getpid());
    }
    free(buffer);
    fclose(f);

  }

while(wait(NULL) > 0);



}



int main(int argc , char **argv){

  if(argc != 3 ){
    printf("Please input right amount of arugments\n");
    return 0;
  }

  char *path = argv[1];
  char *pattern = argv[2];
  
  if(strlen(pattern) > MAX_SIZE){
    printf("Pattern exeeds max size\n");
    return 0;
  }

  check_directory(path,pattern);


}