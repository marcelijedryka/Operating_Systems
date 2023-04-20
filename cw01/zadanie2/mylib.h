#pragma once

typedef struct{
    char **array;
    int maxsize;
    int current_size;
}mystruct;

extern mystruct * init_size(int size);

extern void count_file(mystruct *s, char *filename);

extern char* show_index(mystruct * s , int index);

extern void delete_index(mystruct * s, int index);

extern void destroy(mystruct * s);


