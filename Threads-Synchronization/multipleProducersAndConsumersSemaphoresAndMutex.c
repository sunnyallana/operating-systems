#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

int buffer[BUFFER_SIZE];
sem_t empty_slots, full_slots;
pthread_mutex_t mutex;

void *producer(void *arg) {
    int item = *((int *)arg);
    printf("Hi I am From Producer Function\n My Argument is %d \n", item);
    sleep(1);
    sem_wait(&empty_slots);
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i] == 0) {
            buffer[i] = item;
            printf("Value Produced %d \n",buffer[i]);
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&full_slots);
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    sem_wait(&full_slots);
    pthread_mutex_lock(&mutex);
    int item = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i] != 0) {
            item = buffer[i];
            buffer[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&empty_slots);
    printf("Consumed item: %d\n", item);
    pthread_exit(NULL);
}

int main() {
    pthread_t producer_threads[NUM_PRODUCERS], consumer_threads[NUM_CONSUMERS];
    int producer_args[NUM_PRODUCERS] = {1, 2};
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&full_slots, 0, 0);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&producer_threads[i], NULL, producer, &producer_args[i]);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_create(&consumer_threads[i], NULL, consumer, NULL);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++)
        pthread_join(producer_threads[i], NULL);
    for (int i = 0; i < NUM_CONSUMERS; i++)
        pthread_join(consumer_threads[i], NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);

    return 0;
}
