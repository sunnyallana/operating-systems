#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_SIZE 27

int main(){
	int shmid;
	key_t key;
	char* shm, *s;
	
	key = 2211;
	
	if(shmid = shm_get(key, MAX_SIZE, IPC_CREAT | 0666) < 0){
		return 1;
	}
	
	if(shm = shm_at(shmid, NULL, 0) == (char*)-1){
		return 1;
	}
	s = shm;
	
	for(s = shm; *s != '\0'; s++){
		putchar(*s);
	}
	*shm = '\0';
	puts("");
	exit(0);	
}
