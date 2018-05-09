//Author: Eric Goodwin
//Project 2 - Current Unix processes
//Class: OS, Evening Section

#include "oss.h"

char * printTimeString();

FILE *fp;
FILE *fpMasterLog;

time_t current_time;
struct tm * time_info;
char timeString[9];


int main (int argc, char *argv[]){
	int numProcesses = atoi(argv[1]);
	int index; //for counting

	printf("In producer\n");
	fp = fopen("producer.log", "w");

//signal handler to catch ctrl c
	if(signal(SIGINT, handle) == SIG_ERR){
		perror("Signal Failed");
	}
	if(signal(SIGALRM, handle) == SIG_ERR){
		perror("Signal Failed");
	}	

	//shared memory keys
	turnKey = 59566;
	bufferKey = 59567;
	flagKey = 59562;
	processesKey = 59564;
	//key_t keyEOFFlag = 59563;

	shmidTurn = shmget(turnKey, SHM_SIZE, 0666);
	shmidBuffer = shmget(bufferKey, 5*(sizeof(Buffer)), 0666);
	shmidFlag = shmget(flagKey, 18*SHM_SIZE, 0666);
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


	time_t timeSeed;
	srand((int)time(&timeSeed) % getpid()); //%getpid used because children were all getting the same "random" time to run. 




	//fprintf(fp, "%s\tStarting\n", timeString = printTimeString());
	printf("Start of Produer\n");

	printf("Process %d is a producer = %d\n and consumer is %d\n", getpid(), processes[0].producer, processes[0].consumer);
	//printf("argv[1] is %s\n", argv[1]);


	//signal(SIGINT, handle);

	int i = 0;
	int j;
	int n = numProcesses;

	calculateTime();
	fprintf(fp, "%s\tStarted\n", timeString);
	fflush(fp);
	
	//while (fgets(str, BUFFERSIZE, stdin) != NULL){
	do {
		do{
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
		//
		fpMasterLog = fopen("master.log", "a");
		while (fgets(str, BUFFERSIZE, stdin) != NULL){
			fprintf(fp, "%s\tCheck\n", timeString);
			fflush(fp);

			if (bufferTable[0].isFull == false){
				calculateTime();	
				strcpy(bufferTable[0].data, str);
				bufferTable[0].isFull = true;
				printf("Wrote to buffer 0\n");
				fprintf(fpMasterLog, "PID: %d\tIndex: %d\t'%s'\t Write buffer 0\n", getpid(), 0, bufferTable[0].data);
				fflush(fpMasterLog);
				fprintf(fp, "%s\tWrite\t0\t%s\n", timeString, bufferTable[0].data);
				fflush(fp);
				continue;
			} else if (bufferTable[1].isFull == false) {
				calculateTime();	
	
				strcpy(bufferTable[1].data, str);
				bufferTable[1].isFull = true;
				printf("Wrote to buffer 1\n");
				fprintf(fpMasterLog, "PID: %d\tIndex: %d\t'%s'\t Write buffer 1\n", getpid(), 0, bufferTable[1].data);
				fflush(fpMasterLog);
				fprintf(fp, "%s\tWrite\t1\t%s\n", timeString, bufferTable[1].data);
				fflush(fp);
				continue;
			} else if (bufferTable[2].isFull == false) {
				calculateTime();	

				strcpy(bufferTable[2].data, str);
				bufferTable[2].isFull = true;
				printf("Wrote to buffer 2\n");
				fprintf(fpMasterLog, "PID: %d\tIndex: %d\t'%s'\t Write buffer 2\n", getpid(), 0, bufferTable[2].data);
				fflush(fpMasterLog);
				fprintf(fp, "%s\tWrite\t2\t%s\n", timeString, bufferTable[2].data);
				fflush(fp);
				continue;
			} else if (bufferTable[3].isFull == false) {
				calculateTime();	

				strcpy(bufferTable[3].data, str);
				bufferTable[3].isFull = true;
				printf("Wrote to buffer 3\n");
				fprintf(fpMasterLog, "PID: %d\tIndex: %d\t'%s'\t Write buffer 3\n", getpid(), 0, bufferTable[3].data);
				fflush(fpMasterLog);
				fprintf(fp, "%s\tWrite\t3\t%s\n", timeString, bufferTable[3].data);
				fflush(fp);
				continue;
			} else if (bufferTable[4].isFull == false) {
				calculateTime();	

				strcpy(bufferTable[4].data, str);
				bufferTable[4].isFull = true;
				printf("Wrote to buffer 4\n");
				fprintf(fpMasterLog, "PID: %d\tIndex: %d\t'%s'\t Write buffer 4\n", getpid(), 0, bufferTable[4].data);
				fflush(fpMasterLog);
				fprintf(fp, "%s\tWrite\t4\t%s\n", timeString, bufferTable[4].data);
				fflush(fp);
				continue;
			} else {
				
				fprintf(fpMasterLog, "%d\t%d\tBuffers full on write attempt\n", getpid(), 0);
				fflush(fpMasterLog);
				break;
			}
		}

		fclose(fpMasterLog);
		//Exit section
		
		j = (*turn + 1) % n;
		while (flag[j] == idle)
			j = (j + 1) % n;

		//assign turn to next waiting process; change own flag to idle
		*turn = j; flag[i] = idle;

		//remainder_Section()i
		int randSleep = rand() % 10 + 1;
		calculateTime();
		fprintf(fp, "%s\tSleep\t%d\n", timeString, randSleep);
		fflush(fp);
		printf("Process %d is sleeping for %d\n", getpid(), randSleep);
		sleep(randSleep);
	} while(1);


}



/// functions

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
