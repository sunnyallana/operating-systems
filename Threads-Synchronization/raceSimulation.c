#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define TRACK_LENGTH 100
#define NUM_PARTICIPANTS 5

int track[TRACK_LENGTH];
pthread_mutex_t lock;

typedef struct {
    int id;
    int position;
} Participant;

void *participantRun(void *arg) {
    Participant *p = (Participant *)arg;
    while (p->position < TRACK_LENGTH) {
        pthread_mutex_lock(&lock);
        int move = rand() % 3 + 1; // Random move between 1 and 3 units
        p->position += move;
        printf("Participant %d moved %d units. Position: %d\n", p->id, move, p->position);
        pthread_mutex_unlock(&lock);
        usleep(100000); // Sleep for 100ms
    }
    printf("Participant %d finished the race!\n", p->id);
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL)); // Seed for random numbers
    pthread_t threads[NUM_PARTICIPANTS];
    Participant participants[NUM_PARTICIPANTS];

    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < NUM_PARTICIPANTS; i++) {
        participants[i].id = i + 1;
        participants[i].position = 0;
        pthread_create(&threads[i], NULL, participantRun, (void *)&participants[i]);
    }

    for (int i = 0; i < NUM_PARTICIPANTS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    return 0;
}
