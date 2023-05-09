#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

const int grid_width = 30;
const int grid_height = 30;

char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(char *src, char *dst)
{
    for (int i = 0; i < grid_height; ++i)
    {
        for (int j = 0; j < grid_width; ++j)
        {
            dst[i * grid_width + j] = is_alive(i, j, src);
        }
    }
}

typedef struct {
    char *src;
    char *dst;
    int start;
    int stop;
} gridArgs;

void handler(int signum, siginfo_t* si, void* p3) {}

void *update_grid_2(void *grid_args){

    gridArgs *args = (gridArgs*) grid_args;
    char *tmp;
    
    while (1) {
        for (int i = args->start ; i < args->stop ; i++) {

            args->dst[i] = is_alive(i / grid_width, i % grid_width, args->src);

        }

        pause();

        tmp = args->src;
		args->src = args->dst;
		args->dst = tmp;

    }
}

void update_grid_multithread(char *src, char *dst){

    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = handler;
    sigaction(SIGUSR1, &action, NULL);
    int n = grid_width*grid_height;
    pthread_t *threads = malloc(sizeof(pthread_t) * n);

    for(int i=0 ; i < n ; i++){

        gridArgs *args = malloc(sizeof(gridArgs));

        args->src = src;
        args->dst = dst;
        args->start = i;
        args->stop = i+1;

        pthread_create(threads+i,NULL,update_grid_2,(void*) args);

    }

    for (int i=0 ; i < n ; i++) {
        pthread_kill(threads[i], SIGUSR1);
    }


}


