#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_PHILOSOPHERS 5
#define LEFT (philosopher_number + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS
#define RIGHT (philosopher_number + 1) % NUM_PHILOSOPHERS
#define THINKING 0
#define HUNGRY 1
#define EATING 2

sem_t semaphore;
int state[NUM_PHILOSOPHERS];

void test(int philosopher_number) {
    if (state[philosopher_number] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[philosopher_number] = EATING;
        fprintf(stderr, "Philosopher %d picks up chopsticks %d and %d\n", philosopher_number + 1, LEFT + 1, philosopher_number + 1);
        fprintf(stderr, "Philosopher %d is Eating\n", philosopher_number + 1);
    }
}

void grab_forks(int philosopher_number) {
    sem_wait(&semaphore);
    state[philosopher_number] = HUNGRY;
    fprintf(stderr, "Philosopher %d is Hungry\n", philosopher_number + 1);
    test(philosopher_number);
    sem_post(&semaphore);
}

void put_away_forks(int philosopher_number) {
    sem_wait(&semaphore);
    state[philosopher_number] = THINKING;
    fprintf(stderr, "Philosopher %d puts down chopsticks %d and %d\n", philosopher_number + 1, LEFT + 1, philosopher_number + 1);
    fprintf(stderr, "Philosopher %d is thinking\n", philosopher_number + 1);
    test(LEFT);
    test(RIGHT);
    sem_post(&semaphore);
}

void *philosopher(void *arg) {
    int philosopher_number = *(int *)arg;
    while (1) {
        sleep(rand() % 3); // Thinking
        grab_forks(philosopher_number);
        sleep(rand() % 3); // Eating
        put_away_forks(philosopher_number);
    }
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    sem_init(&semaphore, 0, 1);
    int i, philosopher_number[NUM_PHILOSOPHERS];
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_number[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_number[i]);
    }
    for (i = 0; i < NUM_PHILOSOPHERS; i++)
        pthread_join(philosophers[i], NULL);
    sem_destroy(&semaphore);
    return 0;
}
