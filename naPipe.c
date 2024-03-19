#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 256


int main(void){
	int fd;
	char* myfifo = "/tmp/myfifo";
	
	mkfifo(myfifo, 0666);
	char arr1[80], arr2[80];
	
	while(1)
	{
		fd = open(myfifo, O_WRONLY);
		fgets(arr2, 80, stdin);
		
		write(fd, arr2, strlen(arr2)+1);
		close(fd);
		
		fd = open(myfifo, O_RDONLY);
		read(fd, arr1, sizeof(arr1));
		
		printf("User2: %s\n", arr1);
		close(fd);
	}
	return 0;
}
