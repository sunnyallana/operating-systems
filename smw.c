#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    const int SIZE = 4096;
    const char *name = "OS";
    const char *message0 = "Hello";
    const char *message1 = "World!";
    int shm_fd;
    char *ptr;

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        printf("Shared memory failed\n");
        return 1;
    }

    ftruncate(shm_fd, SIZE);

    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
        return 1;
    }

    sprintf(ptr, "%s", message0);
    ptr += strlen(message0) + 1;
    sprintf(ptr, "%s", message1);

    close(shm_fd);

    return 0;
}

