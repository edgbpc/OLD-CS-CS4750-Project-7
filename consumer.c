//Author: Eric Goodwin
//Project 2 - Current Unix processes
//Class: OS, Evening Section

#include "oss.h"

FILE *fp;
FILE *fpMasterLog;

time_t current_time;
struct tm * time_info;
char timeString[9];

int main (int argc, char *argv[]){

//signal handler to catch ctrl c
	if(signal(SIGINT, handle) == SIG_ERR){
		perror("Signal Failed");
	}
	if(signal(SIGALRM, handle) == SIG_ERR){
		perror("Signal Failed");
	}	

	int location = atoi(argv[1]);
	int numProcesses = atoi(argv[2]);


	//shared memory keys
	turnKey = 59566;
	bufferKey = 59567;
	flagKey = 59562;
	processesKey =  59564;

	shmidTurn = shmget(turnKey, SHM_SIZE, 0666);
	shmidBuffer = shmget(bufferKey, 5*(sizeof(Buffer)), 0666);
	shmidFlag = shmget(flagKey, 18*SHM_SIZE, 0666);
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
	sprintf(fileName, "%s%s.log", consumer, argv[1]);
	fp = fopen(fileName, "w");

	time_t timeSeed;
	srand((int)time(&timeSeed) % getpid()); //%getpid used because children were all getting the same "random" time to run. 

	calculateTime();
	fprintf(fp, "%s\tStarted\n", timeString);
	fflush(fp);

	int i = location;
	int j;
	int n = numProcesses;



	do { 
		do{
			calculateTime();
			printf("Process %d is wants in to the critical section\n", getpid());
			fprintf(fp, "%s\t%d\tAttempt to enter critical section\n", timeString, location);
			fflush(fp);
			flag[i] = want_in;
			j = *turn;
			while (j != i)
				j = (flag[j] != idle ) ? *turn : (j + 1 ) % n;

			flag[i] = in_cs;

			for (j = 0; j < n; j++)
				if ((j != i) && (flag[j] == in_cs))
					break;
		} while ((j < n ) || ( *turn != i && flag[*turn] != idle));
		*turn = i;
		//critical section
		printf("Process %d got into critical section\n", getpid());
		fpMasterLog = fopen("master.log", "a");
		fprintf(fpMasterLog, "PID: %d\tIndex %d \t Attempting to Write to Master Log\n", getpid(), location);
		fflush(fpMasterLog);
		calculateTime();
		fprintf(fp, "%s\tCheck\n", timeString);
		fflush(fp);	
	
		if (bufferTable[0].isFull == true){
			calculateTime();
			printf("Consumer %d is reading from buffer 0\n", location);
			fprintf(fpMasterLog, "PID: %d\tIndex %d '%s'\t Read buffer 0\n", getpid(), location, bufferTable[0].data);
			fflush(fpMasterLog);
			fprintf(fp, "%s\tWrote master log\n", timeString);
			fprintf(fp, "%s\tRead\t0\t%s\n", timeString, bufferTable[0].data);
			fflush(fp);
			bufferTable[0].isFull = false;
		} else if (bufferTable[1].isFull == true){
			calculateTime();
			printf("Consumer %d is reading from buffer 1\n", location);
			fprintf(fpMasterLog, "PID: %d\tIndex: %d '%s'\t Read buffer 1\n", getpid(), location, bufferTable[1].data);
			fflush(fpMasterLog);
			fprintf(fp, "%s\tWrote master log\n", timeString);
			fprintf(fp, "%s\tRead\t1\t%s\n", timeString, bufferTable[1].data);
			fflush(fp);
			bufferTable[1].isFull = false;

		} else if (bufferTable[2].isFull == true){
			printf("Consumer %d is reading from buffer 2\n", location);
			calculateTime();
			fprintf(fpMasterLog, "PID:%d\tIndex:%d '%s'\t Read buffer 2\n", getpid(), location, bufferTable[2].data);
			fflush(fpMasterLog);
			fprintf(fp, "%s\tWrote master log\n", timeString);
			fprintf(fp, "%s\tRead\t2\t%s\n", timeString, bufferTable[2].data);
			fflush(fp);
			bufferTable[2].isFull = false;

		} else if (bufferTable[3].isFull == true){
			printf("Consumer %d is reading from buffer 3\n", location);
			calculateTime();
			fprintf(fpMasterLog, "PID: %d\tIndex:%d '%s'\t Read buffer 3\n", getpid(), location, bufferTable[3].data);
			fflush(fpMasterLog);
			fprintf(fp, "%s\tWrote master log\n", timeString);
			fprintf(fp, "%s\tRead\t3\t%s\n", timeString, bufferTable[3].data);
			fflush(fp);
			bufferTable[3].isFull = false;
		
		} else if (bufferTable[4].isFull == true) {
			calculateTime();
			printf("Consumer %d is reading from buffer 4\n", location);
			fprintf(fpMasterLog, "PID: %d\tIndex: %d '%s'\t Read buffer 4\n", getpid(), location, bufferTable[4].data);
			fflush(fpMasterLog);
			fprintf(fp, "%s\tWrote master log\n", timeString);
			fprintf(fp, "%s\tRead\t4\t%s\n", timeString, bufferTable[4].data);
			fflush(fp);
			bufferTable[4].isFull = false;

		}


		//Exit section
		printf("Process %d is exiting the critical section\n", getpid());
		fclose(fpMasterLog);
		j = (*turn + 1) % n;
		while (flag[j] == idle)
			j = (j + 1) % n;

		//assign turn to next waiting process; change own flag to idle
		*turn = j; flag[i] = idle;


		//remainder_Section()
		
		if (processes[0].completed == true){
			calculateTime();
			fprintf(fp,"%s\tTerminated\tNormal\n", timeString);
			fflush(fp);
			exit(1);
		}
		calculateTime();
		int randSleep = rand() % 5 + 1;
		fprintf(fp, "%s\tSleep\t%d\n", timeString, randSleep);
		fflush(fp);
		sleep(randSleep);

		} while (1);	
}	
	
void calculateTime(){
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
}

void handle(int signo){
	if (signo == SIGINT || signo == SIGALRM){
		calculateTime();
		fprintf(fp, "%s\tTerminated\t Killed\n", timeString);
		fflush(fp);
		printf("process %d received signal\n", getpid());
		exit(1);
	}
}

//BEGIN CRITICAL SECTION//
/*	
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
*/




