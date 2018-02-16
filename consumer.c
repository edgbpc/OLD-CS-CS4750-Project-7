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

#define SHM_SIZE 1000
#define BUFFERSIZE 50

typedef struct {
	char buffer[BUFFERSIZE];
	int flag;
}bufferType;

int main (int argc, char *argv[]){

bufferType bufferOne, *bufferOnePtr;


key_t keyTurn = 59566;

key_t keyBufferOne = 59567;
key_t keyBufferTwo = 59568;
key_t keyBufferThree = 59569;
key_t keyBufferFour = 59560;
key_t keyBufferFive = 59561;


int shmidTurn = shmget(keyTurn, SHM_SIZE, 0777);

int shmidBufferOne = shmget(keyBufferOne, SHM_SIZE, 0777);
//int shmidBufferTwo = shmget(keyBufferTwo, SHM_SIZE, 0777);
//int shmidBufferThree = shmget(keyBufferThree, SHM_SIZ, 0777);
//int shmidBufferFour = shmget(keyBufferFour, SHM_SIZE, 0777);
//int shmidBufferFive = shmget(keyBufferFive, SHM_SIZE, 0777);

int * turn = (int *) (shmat(shmidTurn, 0, 0));

bufferOnePtr = (bufferType *)(shmat (shmidBufferOne, 0, 0));


//bufferOnePtr = &bufferOne;

printf("In Child - Flag value is %s :::::\n ", bufferOnePtr->buffer); 
printf("In Child - Turn: %d :::::::::::::\n ", *turn);


return 1;
}

