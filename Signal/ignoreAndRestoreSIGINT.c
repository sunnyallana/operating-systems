#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    void (*oldHandler)(int);

    printf("I can be Control-C'ed \n");

    sleep(5);

    oldHandler = signal(SIGINT, SIG_IGN); // Returns a function pointer of old handler

    if (oldHandler == SIG_ERR) {
        printf("Error setting signal handler for SIGINT.\n");
        return 1;
    }

    printf("Ctrl-C will be ignored for 5 seconds. Press Ctrl-C to test.\n");

    sleep(5);

    if (signal(SIGINT, oldHandler) == SIG_ERR) {
        printf("Error restoring signal handler for SIGINT.\n");
        return 1;
    }

    printf("Ctrl-C is now restored.\n");

    while (1) {}

    return 0;
}