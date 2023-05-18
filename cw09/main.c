#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define NO_REINDEERS 9
#define NO_ELVES 10

pthread_mutex_t mutex_santa = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_reindeer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_reindeer_delivering = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_elf = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_elf_waiting = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond_santa = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_reindeer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_reindeer_delivering = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_elf = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_elf_waiting = PTHREAD_COND_INITIALIZER;

int reindeers_waiting = 0;
bool delivery_flag = false;
int elves_waiting = 0;
int elf_queue[3];
int delivery_counter = 0;

void clear_queue(int *queue){
    for(int i = 0 ; i < 3 ; i++){
        queue[i] = -1;
    }
}

void *elf(void *arg){

    int id = *((int *) arg);

    while(1){

        sleep(rand() % 4 + 2);

        pthread_mutex_lock(&mutex_elf_waiting);

        while(elves_waiting == 3){
            printf("Elf: samodzielnie rozwiązuję swój problem, ID : %d\n" , id);
            pthread_cond_wait(&cond_elf_waiting, &mutex_elf_waiting);
        }

        pthread_mutex_unlock(&mutex_elf_waiting);

        pthread_mutex_lock(&mutex_elf);
        

        if (elves_waiting < 3){

            elf_queue[elves_waiting] = id;
            elves_waiting++;
            printf("Elf: czeka %d elfów na Mikołaja, ID : %d\n" , elves_waiting , id);

            if(elves_waiting == 3){

                printf("Elf: wybudzam Mikołaja, ID : %d\n" , id);

                pthread_mutex_lock(&mutex_santa);
                pthread_cond_broadcast(&cond_santa);
                pthread_mutex_unlock(&mutex_santa);

            }
        }

        pthread_mutex_unlock(&mutex_elf);

    }

}

void *reindeer(void *arg){

    int id = *((int *) arg);

    while(1){

        pthread_mutex_lock(&mutex_reindeer_delivering);

        while (delivery_flag) {
            pthread_cond_wait(&cond_reindeer_delivering, &mutex_reindeer_delivering);
        }

        pthread_mutex_unlock(&mutex_reindeer_delivering);

        sleep(rand() % 6 + 5);

        

        pthread_mutex_lock(&mutex_reindeer);

        reindeers_waiting ++;

        delivery_flag = true;

        printf("Renifer: czeka %d reniferów na Mikołaja, ID : %d\n" , reindeers_waiting , id);

        if(reindeers_waiting == NO_REINDEERS){
            printf("Renifer: wybudzam Mikołaja, ID : %d\n" , id);

            pthread_mutex_lock(&mutex_santa);
            pthread_cond_broadcast(&cond_santa);
            pthread_mutex_unlock(&mutex_santa);
        }

        pthread_mutex_unlock(&mutex_reindeer);

    }

}


void *santa(void *arg){
    while(1){

        pthread_mutex_lock(&mutex_santa);

        while (elves_waiting < 3 && reindeers_waiting < NO_REINDEERS) {
            pthread_cond_wait(&cond_santa, &mutex_santa);
        }

        pthread_mutex_unlock(&mutex_santa);

        printf("Mikołaj : budzę się\n");

        pthread_mutex_lock(&mutex_elf);

        if(elves_waiting == 3){
            printf("Mikołaj: rozwiązuje problemy elfów %d , %d , %d\n", elf_queue[0], elf_queue[1], elf_queue[2]);

            for(int i = 0 ; i < 3 ; i++){
                sleep(rand() % 2 + 1);
                printf("Elf: Mikołaj rozwiązuje problem, ID : %d\n" , elf_queue[i]);
            }
            

            pthread_mutex_lock(&mutex_elf_waiting);
            elves_waiting = 0;
            clear_queue(elf_queue);
            pthread_cond_broadcast(&cond_elf_waiting);
            pthread_mutex_unlock(&mutex_elf_waiting);

        }

        pthread_mutex_unlock(&mutex_elf);

        pthread_mutex_lock(&mutex_reindeer);

        if(reindeers_waiting == NO_REINDEERS){

            delivery_counter++;
            printf("Mikołaj: dostarczam zabawki - kurs %d\n" , delivery_counter);
            sleep(rand() % 3 + 2);

            reindeers_waiting = 0;
            pthread_mutex_lock(&mutex_reindeer_delivering);
            delivery_flag = false;
            pthread_cond_broadcast(&cond_reindeer_delivering);
            pthread_mutex_unlock(&mutex_reindeer_delivering);

        }

        pthread_mutex_unlock(&mutex_reindeer);

        if(delivery_counter == 3) {
            break;
        }

        printf("Mikołaj: zasypiam\n");

    }

    exit(0);

}


int main(){
    srand(time(NULL));

    pthread_t santa_t;
    pthread_create(&santa_t, NULL, &santa, NULL);

    int *elves_IDs = calloc(NO_ELVES, sizeof(int));
    pthread_t *elf_t = calloc(NO_ELVES, sizeof(pthread_t));

    int *reindeers_IDs = calloc(NO_REINDEERS, sizeof(int));
    pthread_t *reindeer_t = calloc(NO_REINDEERS,  sizeof(pthread_t));

    for(int i = 0 ; i < NO_ELVES ; i++){

        elves_IDs[i] = i;
        pthread_create(&elf_t[i], NULL, &elf, &elves_IDs[i]);

    }

    for(int i = 0 ; i < NO_REINDEERS ; i++){

        reindeers_IDs[i] = i;
        pthread_create(&reindeer_t[i], NULL, &reindeer, &reindeers_IDs[i]);

    }

    pthread_join(santa_t, NULL);

    for(int i = 0; i < NO_ELVES ; i++){

        pthread_join(elf_t[i], NULL);

    }

    for(int i = 0; i < NO_REINDEERS ; i++){

        pthread_join(reindeer_t[i], NULL);

    }

}







