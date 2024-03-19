#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

int main() {
    const int SIZE = 4096;
    const char *name = "OS";
    int fd;
    char *ptr;

    fd = shm_open(name, O_RDONLY, 0666);
    if (fd == -1) {
        printf("Shared memory failed\n");
        return 1;
    }

    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
        return 1;
    }

    printf("%s\n", ptr);

    if (munmap(ptr, SIZE) == -1) {
        printf("Unmap failed\n");
        return 1;
    }

    close(fd);

    if (shm_unlink(name) == -1) {
        printf("Error removing shared memory\n");
        return 1;
    }

    return 0;
}
