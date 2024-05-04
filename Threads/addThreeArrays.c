#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10000000
#define THREADS 10

float *A, *B, *C;
pthread_t tid[THREADS];
int partitionSize;

void* add_arrays(void* arg) {
    int start = *((int*)arg);
    int end = start + partitionSize;
    int i;
    for (i = start; i < end; i++) {
        C[i] = A[i] + B[i];
    }
    return NULL;
}

int main(void) {

    int i, start[THREADS];

    srand(time(NULL));
        
    A = (float*)malloc(SIZE * sizeof(float));
    B = (float*)malloc(SIZE * sizeof(float));
    C = (float*)malloc(SIZE * sizeof(float));

    for (i = 0; i < SIZE; i++) {
        A[i] = (float)(rand() % SIZE);
        B[i] = (float)(rand() % SIZE);
    }

    partitionSize = SIZE / THREADS;
    for (i = 0; i < THREADS; i++) {
        start[i] = i * partitionSize;
        pthread_create(&tid[i], NULL, add_arrays, &start[i]);
    }

    for (i = 0; i < THREADS; i++) {
        pthread_join(tid[i], NULL);
    }
    
    for (i = 0; i < SIZE; i++) {
        printf("%f\n", B[i]);
    }


    free(A);
    free(B);
    free(C);

    return 0;
}