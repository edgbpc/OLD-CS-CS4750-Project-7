//Author: Eric Goodwin
//Project 2 - Current Unix processes
//Class: OS, Evening Section


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <signal.h>

#define SHM_SIZE 1000
#define BUFFERSIZE 50

#define empty 0
#define full 1



typedef struct {
	char buffer[BUFFERSIZE];
	int flag;
}bufferType;


typedef enum { idle , want_in , in_cs } state;

void handle(int sig);
char * printTimeString();

FILE *fp;

time_t current_time;
struct tm * time_info;
char timeString[9];


int main (int argc, char *argv[]){
printf("In producer\n");
fp = fopen("producer.log", "w");

//buffers and pointers
bufferType *bufferOnePtr;
bufferType *bufferTwoPtr;
bufferType *bufferThreePtr;
bufferType *bufferFourPtr;
bufferType *bufferFivePtr;

//shared memory keys
key_t keyTurn = 59566;
key_t keyBufferOne = 59567;
key_t keyBufferTwo = 59568;
key_t keyBufferThree = 59569;
key_t keyBufferFour = 59560;
key_t keyBufferFive = 59561;
key_t keyEOFFlag = 59563;
key_t keyFlag = 59562;

int shmidTurn = shmget(keyTurn, SHM_SIZE, 0777);

int shmidBufferOne = shmget(keyBufferOne, SHM_SIZE, 0777);
int shmidBufferTwo = shmget(keyBufferTwo, SHM_SIZE, 0777);
int shmidBufferThree = shmget(keyBufferThree, SHM_SIZE, 0777);
int shmidBufferFour = shmget(keyBufferFour, SHM_SIZE, 0777);
int shmidBufferFive = shmget(keyBufferFive, SHM_SIZE, 0777);
int shmidEOFFlag = shmget(keyEOFFlag, SHM_SIZE, 0777);
int shmidFlag = shmget(keyFlag, SHM_SIZE, 0777);


int * turn = (int *) (shmat(shmidTurn, 0, 0));
int * EOFFlag = (int *) (shmat(shmidEOFFlag, 0, 0));
state * flag = (state * ) (shmat(shmidFlag, 0, 0));

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


double time_spent;
char str[BUFFERSIZE];


srand(time(NULL));


time(&current_time);
time_info = localtime(&current_time);
strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);


//fprintf(fp, "%s\tStarting\n", timeString = printTimeString());
printf("Start of Produer\n");
//printf("argv[1] is %s\n", argv[1]);


signal(SIGINT, handle);

for ( ; ; ){
//printf("from producer  argv 0 is %s\n", argv[0]);
	int i = atoi(argv[0]);
//	int i = 0;
	int j;
	int n = atoi(argv[1])+1;

//	int n = 2;
	
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


while (fgets(str, BUFFERSIZE, stdin) != NULL) {
	fprintf(fp, "%s\tCheck\n", timeString);
	
	if (bufferOnePtr->flag == empty){
		strcpy(bufferOnePtr->buffer, str);
		bufferOnePtr->flag = full;
	//	printf("Contents of buffer One:%s\n", bufferOnePtr->buffer);
		fprintf(fp, "%s\tWrite\t1\t%s\n", timeString, bufferOnePtr->buffer);
	} else
	if (bufferTwoPtr->flag == empty) {
		strcpy(bufferTwoPtr->buffer, str);
		bufferTwoPtr->flag = full;
	//	printf("Contents of buffer two: %s\n", bufferTwoPtr->buffer);
		fprintf(fp, "%s\tWrite\t2\t%s\n",timeString, bufferTwoPtr->buffer);

	} else
	if (bufferThreePtr->flag == empty) {
		strcpy(bufferThreePtr->buffer, str);
		bufferThreePtr->flag = full;
	//	printf("Contents of buffer three: %s\n", bufferThreePtr->buffer);	
		fprintf(fp, "%s\tWrite\t3\t%s\n",timeString, bufferThreePtr->buffer);

	} else
	if (bufferFourPtr->flag == empty){
		strcpy(bufferFourPtr->buffer, str);
		bufferFourPtr->flag = full;
	//	printf("Contents of buffer four: %s\n", bufferFourPtr->buffer);	
		fprintf(fp, "%s\tWrite\t4\t%s\n",timeString, bufferFourPtr->buffer);
	} else
	if (bufferFivePtr->flag == empty){ 
		strcpy(bufferFivePtr->buffer, str);
		bufferFivePtr->flag = full;
	//	printf("Contents of buffer five: %s\n", bufferFivePtr->buffer);
		fprintf(fp, "%s\tWrite\t5\t%s\n", timeString, bufferFivePtr->buffer);
}
}



/* else if (bufferOnePtr->flag == full && bufferTwoPtr->flag == full && bufferThreePtr->flag == full & bufferFourPtr->flag == full && bufferFivePtr->flag == full){
		clock_t time;
		time = clock();
		do {
			time = clock() - time;
			time_spent = ((double)time)/CLOCKS_PER_SEC;
		}while(bufferOnePtr->flag && bufferTwoPtr->flag && bufferThreePtr->flag && bufferFourPtr->flag && bufferFivePtr->flag);
		fprintf(fp, "%s\tSleep\t%f\n", printTimeString(), time_spent);	
	}*/
//}
//printf("From Producer - end of file\n");
*EOFFlag = 0;
/*	
if (*EOFFlag == 0){
	fprintf(fp, "%s\tTerminated\tNormal Termination", printTimeString());
	exit(2);
	}
*/


//printf("and end of critical section in producer\n");

j = (*turn + 1) % n;
while (flag[j] == idle)
	j = (j + 1) % n;
*turn = j; 
flag[i]=idle;

int r3=((rand()%5)+1);

sleep(r3);
}while (1);

}
exit(2);
}


/// fucntions
void handle(int sig){
	fprintf(fp, "\tTermination\tKilled\n");
	printf("from producer - signal received");
	exit(2);
}

char * printTimeString(){
//	char * timeString = malloc(15);

	return timeString;
	
}
