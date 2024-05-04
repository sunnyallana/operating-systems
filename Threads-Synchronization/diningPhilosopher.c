#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define LEFT (philosopher_number + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS
#define RIGHT (philosopher_number + 1) % NUM_PHILOSOPHERS
#define THINKING 0
#define HUNGRY 1
#define EATING 2

pthread_mutex_t mutex;
pthread_cond_t condition[NUM_PHILOSOPHERS];
int state[NUM_PHILOSOPHERS];

void grab_forks(int philosopher_number) {
    pthread_mutex_lock(&mutex);
    state[philosopher_number] = HUNGRY;
    printf("Philosopher %d is Hungry\n", philosopher_number + 1);
    test(philosopher_number);
    while (state[philosopher_number] != EATING)
        pthread_cond_wait(&condition[philosopher_number], &mutex);
    pthread_mutex_unlock(&mutex);
}

void put_away_forks(int philosopher_number) {
    pthread_mutex_lock(&mutex);
    state[philosopher_number] = THINKING;
    printf("Philosopher %d puts down chopsticks %d and %d\n", philosopher_number + 1, LEFT + 1, philosopher_number + 1);
    printf("Philosopher %d is thinking\n", philosopher_number + 1);
    test(LEFT);
    test(RIGHT);
    pthread_mutex_unlock(&mutex);
}

void test(int philosopher_number) {
    if (state[philosopher_number] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[philosopher_number] = EATING;
        printf("Philosopher %d picks up chopsticks %d and %d\n", philosopher_number + 1, LEFT + 1, philosopher_number + 1);
        printf("Philosopher %d is Eating\n", philosopher_number + 1);
        pthread_cond_signal(&condition[philosopher_number]);
    }
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
    pthread_mutex_init(&mutex, NULL);
    int i, philosopher_number[NUM_PHILOSOPHERS];
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_cond_init(&condition[i], NULL);
        philosopher_number[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_number[i]);
    }
    for (i = 0; i < NUM_PHILOSOPHERS; i++)
        pthread_join(philosophers[i], NULL);
    pthread_mutex_destroy(&mutex);
    for (i = 0; i < NUM_PHILOSOPHERS; i++)
        pthread_cond_destroy(&condition[i]);
    return 0;
}
