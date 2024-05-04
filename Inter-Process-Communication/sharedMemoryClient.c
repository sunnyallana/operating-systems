#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h> // Include ctype.h for the toupper and tolower functions

#define MAX_SIZE 27

int main(){
    int shmid;
    key_t key;
    char* shm, *s;
    
    key = 2211;
    
    if((shmid = shmget(key, MAX_SIZE, IPC_CREAT | 0666)) < 0){ // Corrected the syntax
        perror("shmget");
        exit(1);
    }
    
    if((shm = shmat(shmid, NULL, 0)) == (char*)-1){ // Corrected the syntax
        perror("shmat");
        exit(1);
    }
    s = shm;
    
    for(s = shm; *s != '\0'; s++){
        if(islower(*s))
            putchar(toupper(*s));
        else if(isupper(*s))
            putchar(tolower(*s));
        else
            putchar(*s);
    }
    putchar('\n');
	
    *shm = '\0';
	
    exit(0);    
}
