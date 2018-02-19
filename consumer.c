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


typedef enum { 
	idle, want_in, in_cs
	} state;

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
key_t keyFlag = 59562;
key_t keyEOFFlag = 59563;

int shmidTurn = shmget(keyTurn, SHM_SIZE, 0777);

int shmidBufferOne = shmget(keyBufferOne, SHM_SIZE, 0777);
int shmidBufferTwo = shmget(keyBufferTwo, SHM_SIZE, 0777);
int shmidBufferThree = shmget(keyBufferThree, SHM_SIZE, 0777);
int shmidBufferFour = shmget(keyBufferFour, SHM_SIZE, 0777);
int shmidBufferFive = shmget(keyBufferFive, SHM_SIZE, 0777);
int shmidFlag = shmget(keyFlag, SHM_SIZE, 0777);
int shmidEOFFlag = shmget(keyEOFFlag, SHM_SIZE, 0777);

int * turn = (int *) (shmat(shmidTurn, 0, 0));
state * flag = (state *) (shmat(shmidFlag, 0, 0));
int * EOFFlag = (int *) (shmat(shmidEOFFlag, 0, 0));

bufferOnePtr = (bufferType *)(shmat (shmidBufferOne, 0, 0));
bufferTwoPtr = (bufferType *)(shmat (shmidBufferTwo, 0, 0));
bufferThreePtr = (bufferType *)(shmat (shmidBufferThree, 0, 0));
bufferFourPtr = (bufferType *)(shmat (shmidBufferFour, 0, 0));
bufferFivePtr = (bufferType *)(shmat (shmidBufferFive, 0, 0));

//file
FILE * fp;
char consumer[10] = "consumer";
char fileName[16];
sprintf(fileName, "%s%s.log", consumer, argv[0]);
fp = fopen(fileName, "w");

printf("%s", argv[0]);

//timei
char timeString[9];
time_t secs = time(0);
struct tm *local = localtime(&secs);
sprintf(timeString, "%02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);


fprintf(fp, "%s\tStarted\n", timeString);

for ( ; ; ){

printf("Consumer %s is entering multiple process solution code\n", argv[0]);
srand(time(NULL));
int r;
r=((rand()%5));
//sleep(r);

	int i;
	int j;
	int n = atoi(argv[0]);
	do {
	    do{
		flag[i] = want_in;
		j = *turn;
		while ( j != i )
		    j =  (flag[j] != idle) ? *turn : (j + 1) % n;

		    flag[i] = in_cs;

		    for ( j = 0; j < n; j++)
			if (( j !=i ) && (flag[j] == in_cs))
			break;
		} while ((j < n) || ( *turn != i && flag[*turn] != idle));

		*turn = i;
	


//BEGIN CRITICAL SECTION//


if (bufferOnePtr->flag == full){
	printf("Consumer %s is reading from buffer 1\n", argv[0]);
	fprintf(fp, "%s\tRead\t1\t%s\n", timeString, bufferOnePtr->buffer);
	//strcpy(bufferOnePtr->buffer, "");
	bufferOnePtr->flag = empty;
}

if (bufferTwoPtr->flag == full){
	printf("Consumer %s is reading from reading buffer 2\n", argv[0]);
	fprintf(fp, "%s\tRead\t2\t%s\n", timeString, bufferTwoPtr->buffer);
//	strcpy(bufferTwoPtr->buffer, "");
	bufferTwoPtr->flag = empty;
}
if (bufferThreePtr->flag == full){
	printf("Consumer %s is reading from buffer 3\n", argv[0]);
	fprintf(fp, "%s\tRead\t3\t%s\n", timeString, bufferThreePtr->buffer);
//	strcpy(bufferThreePtr->buffer, "");
	bufferThreePtr->flag = empty;
}
if (bufferFourPtr->flag == full){
	printf("Consumer %s is reading from buffer 4\n", argv[0]);
	fprintf(fp, "%s\tRead\t4\t%s\n", timeString, bufferFourPtr->buffer);
//	strcpy(bufferFourPtr->buffer, "");
	bufferFourPtr->flag = empty;
}
if (bufferFivePtr->flag == full){
	printf("Consumer %s is reading  from buffer 5\n", argv[0]);
	fprintf(fp, "%s\tRead\t5\t%s\n", timeString, bufferFivePtr->buffer);
//	strcpy(bufferFivePtr->buffer, "");
	bufferFivePtr->flag = empty;
}
//eiND CRITICAL SECTION //
//printf("Out of critical section\n");

if (bufferOnePtr->buffer == "" && bufferTwoPtr->buffer == "" && bufferThreePtr->buffer == "" && bufferThreePtr->buffer == "" && bufferFourPtr->buffer == "" && bufferFivePtr->buffer == "") {
printf("No new Data\n");
exit(2);
}
printf("EOFFlag is %d\n", *EOFFlag);
if (*EOFFlag == 0){
printf("From Child - End of file reached");
exit(2);
break;
}

j = (*turn + 1) % n;
while (flag[j] == idle)
	j = (j + 1) % n;
*turn = j; 
flag[i]=idle;

srand(time(NULL));
int r2;
r2=((rand()%5)+1);
sleep(r2);
printf("end of while loop");
}while (1);

}
exit(2);
}
