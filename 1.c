/*stworz 10 watkow, zmienna globalna, watki maja modyfikowac zmienna globalna, losowac liczbe od 1 do 10, 
po 5 sekundach sprawdzic czy jest taka sama jak wylosowal, jezeli tak to konczy dzialanie, jesli nie to ponownie losuje, 
dwa wątki działają naraz - semafor*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 10
#define MIN_VAL 1
#define MAX_VAL 10

int n;

sem_t semaphore;

void *thread_function(void *arg) {
    int thread_id = *((int *)arg);
    free(arg);

    sem_wait(&semaphore);
    srand(time(NULL) + thread_id);

    while (1) {
        int thread_n = (rand() % (MAX_VAL + MIN_VAL - 1));
        n = thread_n;
        printf("Wątek %d wylosował liczbę: %d\n", thread_id, n);
        sleep(5);

        if (thread_n == n) {
            printf("Wylosowana liczba: %d jest rowna n: %d\n", thread_n, n);
            exit(0);
        }
        else {
            printf("Liczba n nie jest rowna liczbie wylosowanej przez wątek\n");
        }
    }

    sem_post(&semaphore);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    sem_init(&semaphore, 0, 2);

    for (int i = 1; i <= NUM_THREADS; i++) {
        sem_wait(&semaphore);
        int *thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&threads[i], NULL, thread_function, thread_id);
    }

    for (int i = 1; i <= NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaphore);

    printf("Program zakończony");
    return 0;
}