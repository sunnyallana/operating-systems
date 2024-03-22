#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256
#define WRITE_END 1
#define READ_END 0


int main() {
    int parent_to_child1[2];
    int parent_to_child2[2];
    int child1_to_parent[2];
    int child2_to_parent[2];

    char buffer[BUFFER_SIZE];

    if (pipe(parent_to_child1) == -1 || pipe(parent_to_child2) == -1 || pipe(child1_to_parent) == -1 || pipe(child2_to_parent) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid_t child1_pid = fork();
    if (child1_pid == -1) {
        perror("Fork failed");
        return 1;
    } else if (child1_pid == 0) {
        close(parent_to_child1[WRITE_END]);
        close(child1_to_parent[READ_END]);

        read(parent_to_child1[READ_END], buffer, BUFFER_SIZE);
        printf("Child 1 received: %s\n", buffer);

        char response[] = "Hello from Windows";
        write(child1_to_parent[WRITE_END], response, strlen(response) + 1);

        close(parent_to_child1[READ_END]);
        close(child1_to_parent[WRITE_END]);
        return 0;
    }

    pid_t child2_pid = fork();
    if (child2_pid == -1) {
        perror("Fork failed");
        return 1;
    } else if (child2_pid == 0) {
        close(parent_to_child2[WRITE_END]);
        close(child2_to_parent[READ_END]);

        read(parent_to_child2[READ_END], buffer, BUFFER_SIZE);
        printf("Child 2 received: %s\n", buffer);

        char response[] = "Hello from Linux";
        write(child2_to_parent[WRITE_END], response, strlen(response) + 1);

        close(parent_to_child2[READ_END]);
        close(child2_to_parent[WRITE_END]);
        return 0;
    }

    close(parent_to_child1[READ_END]);
    close(parent_to_child2[READ_END]);
    close(child1_to_parent[WRITE_END]);
    close(child2_to_parent[WRITE_END]);

    char message1[] = "Message to Child 1 from parent";
    write(parent_to_child1[WRITE_END], message1, strlen(message1) + 1);

    char message2[] = "Message to Child 2 from parent";
    write(parent_to_child2[WRITE_END], message2, strlen(message2) + 1);

    read(child1_to_parent[READ_END], buffer, BUFFER_SIZE);
    printf("Parent received from Child 1: %s\n", buffer);

    read(child2_to_parent[READ_END], buffer, BUFFER_SIZE);
    printf("Parent received from Child 2: %s\n", buffer);

    wait(NULL);
    wait(NULL);

    close(parent_to_child1[WRITE_END]);
    close(parent_to_child2[WRITE_END]);
    close(child1_to_parent[READ_END]);
    close(child2_to_parent[READ_END]);

    return 0;
}

