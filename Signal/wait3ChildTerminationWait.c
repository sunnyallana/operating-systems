#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;

    printf("Parent process (PID: %d)\n", getpid());

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("Child process (PID: %d), Parent process (PPID: %d)\n", getpid(), getppid());
        sleep(2);
        exit(EXIT_SUCCESS);
    } else {
        printf("Parent process waiting for child...\n");
        pid_t terminated_pid = wait3(&status, 0, NULL);
        printf("Child process (PID: %d) terminated with status: %d\n", terminated_pid, status);
    }

    return 0;
}
