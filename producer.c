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

#define empty 0
#define full 1



typedef struct {
	char buffer[BUFFERSIZE];
	int flag;
}bufferType;

int main (int argc, char *argv[]){

//buffers and pointers
bufferType bufferOne, *bufferOnePtr;
bufferType bufferTwo, *bufferTwoPtr;
bufferType bufferThree, *bufferThreePtr;
bufferType bufferFour, *bufferFourPtr;
bufferType bufferFive, *bufferFivePtr;

//shared memory keys
key_t keyTurn = 59566;
key_t keyBufferOne = 59567;
key_t keyBufferTwo = 59568;
key_t keyBufferThree = 59569;
key_t keyBufferFour = 59560;
key_t keyBufferFive = 59561;
key_t keyEOFFlag = 59563;

int shmidTurn = shmget(keyTurn, SHM_SIZE, 0777);

int shmidBufferOne = shmget(keyBufferOne, SHM_SIZE, 0777);
int shmidBufferTwo = shmget(keyBufferTwo, SHM_SIZE, 0777);
int shmidBufferThree = shmget(keyBufferThree, SHM_SIZE, 0777);
int shmidBufferFour = shmget(keyBufferFour, SHM_SIZE, 0777);
int shmidBufferFive = shmget(keyBufferFive, SHM_SIZE, 0777);
int shmidEOFFlag = shmget(keyEOFFlag, SHM_SIZE, 0777);

int * turn = (int *) (shmat(shmidTurn, 0, 0));
int * EOFFlag = (int *) (shmat(shmidEOFFlag, 0, 0));

bufferOnePtr = (bufferType *)(shmat (shmidBufferOne, 0, 0));
bufferTwoPtr = (bufferType *)(shmat (shmidBufferTwo, 0, 0));
bufferThreePtr = (bufferType *)(shmat (shmidBufferThree, 0, 0));
bufferFourPtr = (bufferType *)(shmat (shmidBufferFour, 0, 0));
bufferFivePtr = (bufferType *)(shmat (shmidBufferFive, 0, 0));


//set each buffer flag to empty
bufferOnePtr->flag = empty;
bufferTwoPtr->flag = empty;
bufferThreePtr->flag = empty;
bufferFourPtr->flag = empty;
bufferFivePtr->flag = empty;

//Output File
FILE *fp;

fp = fopen("producer.log", "w");


// time
char timeString[9];
time_t secs = time(0);
struct tm *local = localtime(&secs);
sprintf(timeString, "%02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);


//critical section

char str[BUFFERSIZE];

//fprintf(fp, 

fprintf(fp, "%s\tStarting\n", timeString);
printf("Start of Produer\n");

while (fgets(str, BUFFERSIZE, stdin) != NULL) {
	
	if (bufferOnePtr->flag == empty){
		strcpy(bufferOnePtr->buffer, str);
		bufferOnePtr->flag = full;
//		printf("Contents of buffer One:%s\n", bufferOnePtr->buffer);
		fprintf(fp, "%s\tWrite\t1\t%s\n", timeString, bufferOnePtr->buffer);
	} else
	if (bufferTwoPtr->flag == empty) {
		strcpy(bufferTwoPtr->buffer, str);
		bufferTwoPtr->flag = full;
//		printf("Contents of buffer two: %s\n", bufferTwoPtr->buffer);
		fprintf(fp, "%s\tWrite\t2\t%s", timeString, bufferTwoPtr->buffer);

	} else			
	if (bufferThreePtr->flag == empty) {
		strcpy(bufferThreePtr->buffer, str);
		bufferThreePtr->flag = full;
//		printf("Contents of buffer three: %s\n", bufferThreePtr->buffer);	
		fprintf(fp, "%s\tWrite\t3\t%s", timeString, bufferThreePtr->buffer);

	} else
	if (bufferFourPtr->flag == empty){
		strcpy(bufferFourPtr->buffer, str);
		bufferFourPtr->flag = full;
//		printf("Contents of buffer four: %s\n", bufferFourPtr->buffer);	
		fprintf(fp, "%s\tWrite\t4\t%s", timeString, bufferFourPtr->buffer);
} else
	if (bufferFivePtr->flag == empty){ 
		strcpy(bufferFivePtr->buffer, str);
		bufferFivePtr->flag = full;
//		printf("Contents of buffer five: %s\n", bufferFivePtr->buffer);
		fprintf(fp, "%s\tWrite\t5\t%s\n", timeString, bufferFivePtr->buffer);
	}

	//printf("STR is %s\n", str);

//	if (bufferOnePtr->flag == full && bufferTwoPtr->flag == full && bufferThreePtr->flag == full && bufferFourPtr->flag == full && bufferFivePtr->flag == full) {
	fprintf(fp, "%s\tSleep\n", timeString);	
	do {
		}while(bufferOnePtr->flag && bufferTwoPtr->flag && bufferThreePtr->flag && bufferFourPtr->flag && bufferFivePtr->flag);
}
printf("From Producer - end of file\n");
*EOFFlag = 0;
printf("from Producer - end of file flag is %d\n", *EOFFlag); 

exit(EXIT_SUCCESS);
}
