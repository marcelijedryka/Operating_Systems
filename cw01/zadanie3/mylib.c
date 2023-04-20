#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"

mystruct * init_size(int size){
    mystruct *s = malloc(sizeof(mystruct));
    s->array = (char**)calloc(size,sizeof(char));
    s->maxsize = size;
    s->current_size = 0;
    return s;
}

void count_file(mystruct *s, char *filename){
    char *new_command = (char*) calloc(256, sizeof (char));
    strcat(new_command,"wc ");
    strcat(new_command,filename);
    strcat(new_command,">");
    strcat(new_command,"/tmp/");
    strcat(new_command,filename);
    // printf("Command name: %s ", new_command);
    char *contents = (char*) calloc(256,sizeof (char));
    char *path = (char*) calloc(256, sizeof (char));
    strcat(path,"/tmp/");
    strcat(path,filename);
    system(new_command);
    FILE *file = fopen(path,"r");
    fgets(contents,256,file);
    char *buffer = (char*) calloc(strlen(contents),sizeof(char));
    strcat(buffer,contents);
    s->array[s->current_size] = buffer;
    s->current_size++;
    fclose(file);
    remove(path);
    free(new_command);
    free(contents);
    free(path);
}



char* show_index(mystruct * s , int index){
    return s->array[index];
}

void delete_index(mystruct * s, int index){
    free(s->array[index]);
    s->current_size--;
}

void destroy(mystruct * s){
    free(s->array);
}

