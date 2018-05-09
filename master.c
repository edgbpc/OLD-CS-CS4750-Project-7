//Author: Eric Goodwin
//Project 2 - Current Unix Processes
//Class: OS, Evening section
//
//

#include "oss.h"

pid_t childPid;
Buffer *bufferTable;
Process *processes;

int * turn;
state * flag;

void printUsage();

int main (int argc, char *argv[]){

	double terminateTime = 100; //used by setperiodic to terminate program
	int option;
	int numConsumers = 10;
	int index; //for loop counting
	int status;
	pid_t wpid;

//signal handler to catch ctrl c
	if(signal(SIGINT, handle) == SIG_ERR){
		perror("Signal Failed");
	}
	if(signal(SIGALRM, handle) == SIG_ERR){
		perror("Signal Failed");
	}	
//set timer. from book
	if (setperiodic(terminateTime) == -1){
		perror("oss: failed to set run timer");
		return 1;
	}


	turnKey = 59566;
	bufferKey = 59567;
	flagKey = 59562;
	processesKey = 59564;

	key_t keyEOFFlag = 59563; 


//create shared memory locations
	shmidTurn = shmget(turnKey, SHM_SIZE, IPC_CREAT | 0666);
	shmidBuffer = shmget(bufferKey, 5*(sizeof(Buffer)), IPC_CREAT | 0666);
	shmidFlag = shmget(flagKey, 18*SHM_SIZE, IPC_CREAT | 0666);
	shmidProcesses = shmget(processesKey, 18*(sizeof(Process)), IPC_CREAT | 0666);

//Attach to shared memory
	turn = (int *)shmat(shmidTurn, NULL, 0);
	bufferTable = (Buffer *)shmat(shmidBuffer, NULL, 0);
	flag = (state *)shmat(shmidFlag, NULL, 0);
	processes = (Process *)shmat(shmidProcesses, NULL, 0);



//int * EOFflag = (int *)shmat(shmidEOFFlag, NULL, 0);

//*EOFflag = 1;




	while ((option = getopt(argc, argv, "n:h")) != -1){
		switch (option){
			case 'n' : numConsumers = atoi(optarg);
				   if (numConsumers > 17){
				   	printf("Max processes is 20\n");
					printf("Running with 1 Producer, 17 Consumers\n");
					numConsumers = 17;
					}
				break;

			case 'h' : 
				printUsage();
				terminateSharedResources();
				return 0;
				break;

			default :
				printf("Invalid option selected\n");
				terminateSharedResources();
				return 0;
				break;	
		}
	}	

	printf("No processes forked yet\n");


	//create producer child
	childPid = fork();

	if (childPid == -1){
		printf("Failed to fork.  Aborting");
		exit(0);
	} else if (childPid == 0){
		processes[0].index = 0;
		char intBuffer1[3];
		sprintf(intBuffer1, "%d", numConsumers + 1);
		execl("./producer", "producer", intBuffer1, NULL);
	}	



//create consumer children
	for (index = 1; index <= numConsumers; index++){
		//printf("in consumer creation loop\n");
		childPid = fork();
		if (childPid == -1){
			printf("failed to fork. Aborting");
			exit(0);
		} else if (childPid == 0){
			char intBuffer2[3];
			char intBuffer3[3];
			sprintf(intBuffer2, "%d", index);
			sprintf(intBuffer3, "%d", numConsumers + 1);
			processes[index].index = index;
			execl("./consumer", "consumer", intBuffer2, intBuffer3, NULL);
		 } 
	} 

while ((wpid = wait(&status)) > 0);


return 1;
}

//TAKEN FROM BOOK
static int setperiodic(double sec) {
   timer_t timerid;
   struct itimerspec value;
   if (timer_create(CLOCK_REALTIME, NULL, &timerid) == -1)
      return -1;
   value.it_interval.tv_sec = (long)sec;
   value.it_interval.tv_nsec = (sec - value.it_interval.tv_sec)*BILLION;
   if (value.it_interval.tv_nsec >= BILLION) {
      value.it_interval.tv_sec++;
      value.it_interval.tv_nsec -= BILLION;
   }
   value.it_value = value.it_interval;
   return timer_settime(timerid, 0, &value, NULL);
}
void handle(int signo){
	if (signo == SIGINT || signo == SIGALRM){
		printf("*********Signal was received************\n");
		terminateSharedResources();
		kill(0, SIGKILL);
		wait(NULL);
		exit(0);
	} else {
	}
}

void printUsage(){
	printf("./master options < file\n");
	printf("Options:\n");
	printf("-n to specify number of consumers.  Max 17.  Default 10 if not selected\n");
	printf("-h this help message\n");


}

void terminateSharedResources(){
	shmdt(turn);
	shmdt(flag);
	//shmdt(EOFflag);
	shmdt(bufferTable);
	shmdt(processes);
	shmctl(shmidTurn, IPC_RMID, NULL);
	shmctl(shmidFlag, IPC_RMID, NULL);
	shmctl(shmidBuffer, IPC_RMID, NULL);
	shmctl(shmidProcesses, IPC_RMID, NULL);
}
