#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int signo) {
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
}

int main(void) {
    struct sigaction sa;

    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        printf("Error setting up signal handler for SIGUSR1\n");
    }

    printf("Attempting to set up signal handler for SIGKILL and SIGSTOP...\n");

    if (sigaction(SIGKILL, &sa, NULL) == -1) {
        printf("Error setting up signal handler for SIGKILL\n");
    }

// Ignore the signal. By default, linux doesn't allow to ignore SIGKILL and SIGSTOP.
    if (signal(SIGKILL, SIG_IGN) == SIG_ERR) {
        printf("Cannot ignore SIGKILL.\n");
        return 1;
    }

    if (sigaction(SIGSTOP, &sa, NULL) == -1) {
        printf("Error setting up signal handler for SIGSTOP\n");
    }

    while (1)
        sleep(1);

    return 0;
}