#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t semaphore;
int crossing = 0;

void* chimp(void* arg) {
    int id = *(int*)arg;

    while (1) {
        sem_wait(&semaphore);

        while (crossing != 0 && crossing != id) {
            sem_post(&semaphore);
            sem_wait(&semaphore);
        }

        crossing = id;
        printf("Chimpanzee %d is crossing.\n", id);
        fflush(stdout);

        sem_post(&semaphore);
        usleep(1000000);

        sem_wait(&semaphore);
        crossing = 0;
        printf("Chimpanzee %d has crossed.\n", id);
        fflush(stdout);

        sem_post(&semaphore);

        usleep(1000000);
    }

    return NULL;
}

int main() {
    pthread_t chimp1, chimp2;
    int ids[2] = {1, 2};

    sem_init(&semaphore, 0, 1);

    pthread_create(&chimp1, NULL, chimp, &ids[0]);
    pthread_create(&chimp2, NULL, chimp, &ids[1]);

    pthread_join(chimp1, NULL);
    pthread_join(chimp2, NULL);

    sem_destroy(&semaphore);

    return 0;
}
