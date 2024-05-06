#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t account_mutex = PTHREAD_MUTEX_INITIALIZER;
double account_balance = 1000.0;

void* deposit(void* arg) {
    double amount = *(double*)arg;

    pthread_mutex_lock(&account_mutex);
    account_balance += amount;
    printf("Deposited: %.2f\n", amount);
    pthread_mutex_unlock(&account_mutex);

    return NULL;
}

void* withdraw(void* arg) {
    double amount = *(double*)arg;

    pthread_mutex_lock(&account_mutex);
    if (account_balance >= amount) {
        account_balance -= amount;
        printf("Withdrawn: %.2f\n", amount);
    } else {
        printf("Insufficient funds for withdrawal of %.2f\n", amount);
    }
    pthread_mutex_unlock(&account_mutex);

    return NULL;
}

int main() {
    pthread_t threads[4];
    double deposit_amounts[2] = {200.0, 300.0};
    double withdraw_amounts[2] = {150.0, 800.0};

    pthread_create(&threads[0], NULL, deposit, &deposit_amounts[0]);
    pthread_create(&threads[1], NULL, deposit, &deposit_amounts[1]);
    pthread_create(&threads[2], NULL, withdraw, &withdraw_amounts[0]);
    pthread_create(&threads[3], NULL, withdraw, &withdraw_amounts[1]);

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final account balance: %.2f\n", account_balance);

    return 0;
}
