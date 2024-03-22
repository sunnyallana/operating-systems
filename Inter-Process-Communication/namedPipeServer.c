#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_FILE "/tmp/myfifo"

int main(){
	int fd;
	char buffer[BUFSIZ];
	ssize_t num_bytes;
	
	if(mkfifo(FIFO_FILE, 0666) == -1){
		perror("mkfifo");
		exit(EXIT_FAILURE);
	}
	
	fd = open(FIFO_FILE, O_WRONLY);
	if(fd == -1){
		perror("open");
		exit(EXIT_FAILURE);
	}
	
	while(1){
		printf("Producer: Enter a messsage or exit to quit");
		fgets(buffer, BUFSIZ, stdin);
		num_bytes = write(fd, buffer, strlen(buffer)+1);
		if(num_bytes == -1){
			perror("write");
			exit(EXIT_FAILURE);
		}
		if(strcmp(buffer, "exit") == 0){
			break;
		}
	}
	
	close(fd);
	unlink(FIFO_FILE);
	return 0;
}
	
