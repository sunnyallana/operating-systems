#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>

#define ARRAY_SIZE 1000000
#define NUM_THREADS 4

int array[ARRAY_SIZE];
long long sum = 0;
sem_t semaphore;

void alarm_handler(int sig) {
    sem_post(&semaphore);
}

void *sumArray(void *thread_id) {
    long tid = (long)thread_id;
    long long partial_sum = 0;

    sem_wait(&semaphore);

    for (long i = tid * (ARRAY_SIZE / NUM_THREADS); i < (tid + 1) * (ARRAY_SIZE / NUM_THREADS); ++i) {
        partial_sum += array[i];
    }

    sum += partial_sum;

    sleep(5);

    sem_post(&semaphore);

    pthread_exit(NULL);
}

int main() {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = i + 1;
    }

    sem_init(&semaphore, 0, 1);

    signal(SIGALRM, alarm_handler);

    pthread_t threads[NUM_THREADS];
    long t;

    for (t = 0; t < NUM_THREADS; ++t) {
        pthread_create(&threads[t], NULL, sumArray, (void *)t);
    }

    alarm(5);

    for (t = 0; t < NUM_THREADS; ++t) {
        pthread_join(threads[t], NULL);
    }

    sem_destroy(&semaphore);

    printf("Sum of the array elements: %lld\n", sum);

    return 0;
}   