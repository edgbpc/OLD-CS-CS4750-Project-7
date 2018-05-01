//Author: Eric Goodwin
//Project 2 - Current Unix processes
//Class: OS, Evening Section

#include "oss.h"

FILE *fp;

time_t current_time;
struct tm * time_info;
char timeString[9];

int main (int argc, char *argv[]){

	int location = atoi(argv[1]);

	signal(SIGINT, handle);

	//shared memory keys
	turnKey = 59566;
	bufferKey = 59567;
	flagKey = 59562;
	processesKey =  59564;

	shmidTurn = shmget(turnKey, SHM_SIZE, 0666);
	shmidBuffer = shmget(bufferKey, 5*(sizeof(Buffer)), 0666);
	shmidFlag = shmget(flagKey, SHM_SIZE, 0666);
	shmidProcesses = shmget(processesKey, 18*(sizeof(Process)), 0666);

//	int shmidEOFFlag = shmget(keyEOFFlag, SHM_SIZE, 0777);

	int *turn = (int *) (shmat(shmidTurn, 0, 0));
	state * flag =  (state *) (shmat(shmidFlag, 0, 0));
//	*EOFFlag = (int *) (shmat(shmidEOFFlag, 0, 0));

	Buffer * bufferTable = (Buffer *)(shmat (shmidBuffer, 0, 0));
	Process * processes = (Process *)(shmat(shmidProcesses, 0, 0));

//file
	char consumer[12] = "consumer";
	char fileName[20];
	sprintf(fileName, "%s%s.log", consumer, argv[0]);
	fp = fopen(fileName, "w");

	printf("Process %d consumer flag is %d, producer flag is %d\n", getpid(), processes[location].consumer, processes[location].producer );

/*
	srand(time(NULL));


	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);

	fprintf(fp, "%s\tStarted\n", timeString);

	for ( ; ; ){

		printf("Consumer %s is entering multiple process solution code\n", argv[0]);
		int r;
		r=((rand()%5));
		sleep(r);


		//int i = 1;
		int i = atoi(argv[0])+1;
		int j;
		int n = atoi(argv[1])+1;
		//int n = 2;

		printf("int I is : %d\n", i);
		printf("int n is : %d\n", n);

		do {
		    do{
	//	printf("in the neste do loop\n");
			flag[i] = want_in;
			j = *turn;
			while ( j != i )
			    j =  (flag[j] != idle) ? *turn : (j + 1) % n;
		
			    flag[i] = in_cs;

			    for ( j = 0; j < n; j++)
				if (( j != i ) && (flag[j] == in_cs))
				break;
	//	printf("bottom of the nested do\n");
			} while ((j < n) || ( *turn != i && flag[*turn] != idle));
	//	printf("out of the internal do loop\n");
			*turn = i;
//BEGIN CRITICAL SECTION//
	
		   printf("before consumer reads\n");
 		   if (bufferOnePtr->flag == full){
 			   printf("Consumer %s is reading from buffer 1\n", argv[0]);
			   fprintf(fp, "%s\tRead\t1\t%s\n", timeString, bufferOnePtr->buffer);
				//strcpy(bufferOnePtr->buffer, "");
			   bufferOnePtr->flag = empty;
			   printf("flag is %d", bufferOnePtr->flag);
		 } else

if (bufferTwoPtr->flag == full){
	printf("Consumer %s is reading from reading buffer 2\n", argv[0]);
	fprintf(fp, "%s\tRead\t2\t%s\n",timeString, bufferTwoPtr->buffer);
//	strcpy(bufferTwoPtr->buffer, "");
	bufferTwoPtr->flag = empty;
} else
if (bufferThreePtr->flag == full){
	printf("Consumer %s is reading from buffer 3\n", argv[0]);
	fprintf(fp, "%s\tRead\t3\t%s\n", timeString, bufferThreePtr->buffer);
//	strcpy(bufferThreePtr->buffer, "");
	bufferThreePtr->flag = empty;
} else
if (bufferFourPtr->flag == full){
	printf("Consumer %s is reading from buffer 4\n", argv[0]);
	fprintf(fp, "%s\tRead\t4\t%s\n", timeString, bufferFourPtr->buffer);
//	strcpy(bufferFourPtr->buffer, "");
	bufferFourPtr->flag = empty;
} else
if (bufferFivePtr->flag == full){
	printf("Consumer %s is reading  from buffer 5\n", argv[0]);
	fprintf(fp, "%s\tRead\t5\t%s\n", timeString, bufferFivePtr->buffer);
//	strcpy(bufferFivePtr->buffer, "");
	bufferFivePtr->flag = empty;
 } 

//eiND CRITICAL SECTION //
//printf("Out of critical section\n");

//if (bufferOnePtr->buffer == "" && bufferTwoPtr->buffer == "" && bufferThreePtr->buffer == "" && bufferThreePtr->buffer == "" && bufferFourPtr->buffer == "" && bufferFivePtr->buffer == "") {
//printf("No new Data\n");
//exit(2);
//}
//printf("EOFFlag is %d\n", *EOFFlag);
//if (*EOFFlag == 0){
//printf("From Child - End of file reached");
//exit(2);
//break;

j = (*turn + 1) % n;
while (flag[j] == idle)
	j = (j + 1) % n;
*turn = j; 
flag[i]=idle;

printf("after exit code in Child");


int r2=((rand()%5)+1);
sleep(r2);
//printf("end of while loop");
}while (1);

}
*/
exit(2);
}




void handle(int sig){
	printf("from child - signal received");
	exit(2);
}
