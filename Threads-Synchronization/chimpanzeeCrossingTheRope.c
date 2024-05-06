#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int crossing = 0;

void* chimp(void* arg) {
    int id = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        while (crossing != 0 && crossing != id) {
            pthread_cond_wait(&cond, &mutex);
        }

        crossing = id;
        printf("Chimpanzee %d is crossing.\n", id);
        fflush(stdout);

        pthread_mutex_unlock(&mutex);
        usleep(1000000);

        pthread_mutex_lock(&mutex);
        crossing = 0;
        printf("Chimpanzee %d has crossed.\n", id);
        fflush(stdout);

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);

        usleep(1000000);
    }

    return NULL;
}

int main() {
    pthread_t chimp1, chimp2;
    int ids[2] = {1, 2};

    pthread_create(&chimp1, NULL, chimp, &ids[0]);
    pthread_create(&chimp2, NULL, chimp, &ids[1]);

    pthread_join(chimp1, NULL);
    pthread_join(chimp2, NULL);

    return 0;
}
