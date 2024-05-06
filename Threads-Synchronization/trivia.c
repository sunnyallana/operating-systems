#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_PARTICIPANTS 4
#define MAX_QUESTIONS 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int current_participant = 0;
int scores[NUM_PARTICIPANTS] = {0};

void* participant_function(void* arg) {
    int id = *(int*)arg;
    int questions_answered = 0;

    while (questions_answered < MAX_QUESTIONS) {
        pthread_mutex_lock(&mutex);

        while (current_participant != id) {
            pthread_cond_wait(&cond, &mutex);
        }

        int is_correct = (rand() % 2 == 0) ? 1 : 0;
        if (is_correct) {
            scores[id]++;
        }

        current_participant = (current_participant + 1) % NUM_PARTICIPANTS;
        questions_answered++;
        pthread_cond_broadcast(&cond);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    pthread_t participants[NUM_PARTICIPANTS];
    int participant_ids[NUM_PARTICIPANTS] = {0, 1, 2, 3};

    srand(time(NULL));

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

    return 0;
}
