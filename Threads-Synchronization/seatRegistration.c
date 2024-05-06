#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define THREAD_NUM 5

pthread_mutex_t mutex;
int seats = 3;


struct UserData
{
    char *name;
    int ID;
}userData[THREAD_NUM];


void* register_user(void* args){
    int id = *((int *)args);

    pthread_mutex_lock(&mutex);
    userData[id].ID = id;
    userData[id].name = "Any name";
    if(seats <= 0){
        printf("User ID: %d\n",userData[id].ID);
        printf("Sorry, no seats available\n");
        pthread_mutex_unlock(&mutex);
    }else{
        printf("User ID: %d\n",userData[id].ID);
        printf("Name: %s\n",userData[id].name);
        seats--;
        printf("Registration completed\n\nSeats available: %d\n\n",seats);

        pthread_mutex_unlock(&mutex);
        usleep(1000);
    }
}

int main(){
     pthread_t customers[THREAD_NUM];
     pthread_mutex_init(&mutex,0);

    int threadID[THREAD_NUM];
    for(int i=0;i<THREAD_NUM;i++){
        threadID[i] = i;
        pthread_create(&customers[i],NULL,register_user,(void *)&threadID[i]);
    }

    for(int i = 0;i<THREAD_NUM;i++){
        pthread_join(customers[i],NULL);
    }

    return 0;
}