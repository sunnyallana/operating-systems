#include <stdio.h>
#include <pthread.h>

#define ARRAY_SIZE 1000000
#define NUM_THREADS 4

int array[ARRAY_SIZE];
long long sum = 0;
// pthread_mutex_t mutex_sum = PTHREAD_MUTEX_INITIALIZER;

void *sumArray(void *thread_id) {
    long tid = (long)thread_id;
    long long partial_sum = 0;

    for (long i = tid * (ARRAY_SIZE / NUM_THREADS); i < (tid + 1) * (ARRAY_SIZE / NUM_THREADS); ++i) {
        partial_sum += array[i];
    }

    // pthread_mutex_lock(&mutex_sum);
    sum += partial_sum;
    // pthread_mutex_unlock(&mutex_sum);

    pthread_exit(NULL);
}

int main() {
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = i + 1;
    }

    pthread_t threads[NUM_THREADS];
    long t;

    for (t = 0; t < NUM_THREADS; ++t) {
        pthread_create(&threads[t], NULL, sumArray, (void *)t);
    }

    for (t = 0; t < NUM_THREADS; ++t) {
        pthread_join(threads[t], NULL);
    }

    printf("Sum of the array elements: %lld\n", sum);

    return 0;
}
