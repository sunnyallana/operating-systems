#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int average = 0, maximum = 0, minimum = 0;

void *calculateAverage(void *arg) {
    int *numbers = (int *)arg;
    int sum = 0, i;
    for (i = 0; numbers[i] != -1; ++i) {
        sum += numbers[i];
    }
    average = sum / i;
    pthread_exit(NULL);
}

void *calculateMax(void *arg) {
    int *numbers = (int *)arg;
    maximum = numbers[0];
    int i;
    for (i = 1; numbers[i] != -1; ++i) {
        if (numbers[i] > maximum) {
            maximum = numbers[i];
        }
    }
    pthread_exit(NULL);
}

void *calculateMin(void *arg) {
    int *numbers = (int *)arg;
    minimum = numbers[0];
    int i;
    for (i = 1; numbers[i] != -1; ++i) {
        if (numbers[i] < minimum) {
            minimum = numbers[i];
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number1> <number2> ... <numberN>\n", argv[0]);
        return 1;
    }

    int *numbers = (int *)malloc(sizeof(int) * argc);
    int i;
    for (i = 1; i < argc; ++i) {
        numbers[i - 1] = atoi(argv[i]);
    }
    numbers[argc - 1] = -1;

    pthread_t threads[3];
    pthread_create(&threads[0], NULL, calculateAverage, (void *)numbers);
    pthread_create(&threads[1], NULL, calculateMax, (void *)numbers);
    pthread_create(&threads[2], NULL, calculateMin, (void *)numbers);

    for (i = 0; i < 3; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("The average value is %d.\n", average);
    printf("The maximum value is %d.\n", maximum);
    printf("The minimum value is %d.\n", minimum);

    free(numbers);

    return 0;
}