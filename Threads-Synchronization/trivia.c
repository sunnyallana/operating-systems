#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define NUM_PARTICIPANTS 4
#define MAX_QUESTIONS 10

sem_t semaphores[NUM_PARTICIPANTS];
int scores[NUM_PARTICIPANTS] = {0};

void* participant_function(void* arg) {
    int id = *(int*)arg;
    int questions_answered = 0;

    while (questions_answered < MAX_QUESTIONS) {
        sem_wait(&semaphores[id]);

        int is_correct = (rand() % 2 == 0) ? 1 : 0;
        if (is_correct) {
            scores[id]++;
        }

        questions_answered++;
        sem_post(&semaphores[(id + 1) % NUM_PARTICIPANTS]);
    }

    return NULL;
}

int main() {
    pthread_t participants[NUM_PARTICIPANTS];
    int participant_ids[NUM_PARTICIPANTS] = {0, 1, 2, 3};

    srand(time(NULL));

    for (int i = 0; i < NUM_PARTICIPANTS; i++) {
        sem_init(&semaphores[i], 0, 0);
    }
    sem_post(&semaphores[0]); // Start with the first participant

    for (int i = 0; i < NUM_PARTICIPANTS; i++) {
        pthread_create(&participants[i], NULL, participant_function, &participant_ids[i]);
    }

    for (int i = 0; i < NUM_PARTICIPANTS; i++) {
        pthread_join(participants[i], NULL);
    }

    int max_score = scores[0];
    int winner_id = 0;
    for (int i = 1; i < NUM_PARTICIPANTS; i++) {
        if (scores[i] > max_score) {
            max_score = scores[i];
            winner_id = i;
        }
    }

    printf("Competition ended.\n");
    printf("Final Scores:\n");
    for (int i = 0; i < NUM_PARTICIPANTS; i++) {
        printf("Participant %d | Score: %d\n", i, scores[i]);
    }
    printf("Winner: Participant %d with score %d\n", winner_id, max_score);

    for (int i = 0; i < NUM_PARTICIPANTS; i++) {
        sem_destroy(&semaphores[i]);
    }

    return 0;
}
