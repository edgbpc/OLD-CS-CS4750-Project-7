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

int *turn;
int *flag;

printf("From consumer child\n");

key_t key = 59566;

int shmid = shmget(key, SHM_SIZE, 0777);

turn = (int *)(shmat (shmid, 0, 0 ));
flag = turn +1;

printf("%d\n", *turn);

return 1;

}

