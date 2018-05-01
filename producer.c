//Author: Eric Goodwin
//Project 2 - Current Unix processes
//Class: OS, Evening Section

#include "oss.h"

char * printTimeString();

FILE *fp;

time_t current_time;
struct tm * time_info;
char timeString[9];


int main (int argc, char *argv[]){
printf("In producer\n");
fp = fopen("producer.log", "w");

//shared memory keys
turnKey = 59566;
bufferKey = 59567;
flagKey = 59562;
processesKey = 59564;
//key_t keyEOFFlag = 59563;

shmidTurn = shmget(turnKey, SHM_SIZE, 0666);
shmidBuffer = shmget(bufferKey, 5*(sizeof(Buffer)), 0666);
shmidFlag = shmget(flagKey, SHM_SIZE, 0666);
shmidProcesses = shmget(processesKey, 18*(sizeof(Process)), 0666);

//int shmidEOFFlag = shmget(keyEOFFlag, SHM_SIZE, 0777);


int * turn = (int *) (shmat(shmidTurn, 0, 0));
//int * EOFFlag = (int *) (shmat(shmidEOFFlag, 0, 0));
state * flag = (state * ) (shmat(shmidFlag, 0, 0));
Buffer * bufferTable = (Buffer*)(shmat (shmidBuffer, 0, 0));
Process * processes = (Process *)(shmat(shmidProcesses, 0, 0));

//set each buffer flag to empty


double time_spent;
char str[BUFFERSIZE];


srand(time(NULL));


time(&current_time);
time_info = localtime(&current_time);
strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);


//fprintf(fp, "%s\tStarting\n", timeString = printTimeString());
printf("Start of Produer\n");

printf("Process %d is a producer = %d\n and consumer is %d\n", getpid(), processes[0].producer, processes[0].consumer);
//printf("argv[1] is %s\n", argv[1]);

/*
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



 else if (bufferOnePtr->flag == full && bufferTwoPtr->flag == full && bufferThreePtr->flag == full & bufferFourPtr->flag == full && bufferFivePtr->flag == full){
		clock_t time;
		time = clock();
		do {
			time = clock() - time;
			time_spent = ((double)time)/CLOCKS_PER_SEC;
		}while(bufferOnePtr->flag && bufferTwoPtr->flag && bufferThreePtr->flag && bufferFourPtr->flag && bufferFivePtr->flag);
		fprintf(fp, "%s\tSleep\t%f\n", printTimeString(), time_spent);	
	}
//}
//printf("From Producer - end of file\n");
*EOFFlag = 0;
*	
if (*EOFFlag == 0){
	fprintf(fp, "%s\tTerminated\tNormal Termination", printTimeString());
	exit(2);
	}



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
*/
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
