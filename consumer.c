//Author: Eric Goodwin
//Project 2 - Current Unix processes
//Class: OS, Evening Section


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <string.h>

#define SHM_SIZE 1024

int main (int argc, char *argv[]){

printf("THERE!");

key_t key = 59566;

int shmid = shmget(key, SHM_SIZE, 0777);

int * turn = (int *)(shmat (shmid, 0, 0 ));


printf("%d", *turn);

//return 1;

}

