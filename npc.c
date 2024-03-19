#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_FILE "/tmp/myfifo"

int main(void){
	int fd;
	char buffer[BUFSIZ];
	ssize_t num_bytes;
	
	fd = open(FIFO_FILE, O_RDONLY);
	if(fd == -1){
		perror("Open");
		exit(EXIT_FAILURE);
	}
	
	printf("Consumer: Waiting for messages...\n");
	
	while((num_bytes = read(fd, buffer, BUFSIZ)) > 0){
		buffer[num_bytes] = '\0';
		printf("Consumer: Received message: %s", buffer);
	}
	
	if(num_bytes == -1){
		perror("Read");
		exit(EXIT_FAILURE);
	}
	
	printf("Consumer: End of message\n");
	
	close(fd);
	
	return 0;
} 
		
