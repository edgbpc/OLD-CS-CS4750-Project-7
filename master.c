//Author: Eric Goodwin
//Project 2 - Current Unix Processes
//Class: OS, Evening section
//
//

#include "oss.h"

pid_t childPid;
Buffer *bufferTable;
Process *processes;

int main (int argc, char *argv[]){



//set alarm  timer to 100 seconds.  signal invokes SIGALRM and calls handle to terminate program
alarm(100);
//signal(SIGALRM, handle);
//signal(SIGINT, childTrap);

turnKey = 59566;
bufferKey = 59567;
flagKey = 59562;
processesKey = 59564;

key_t keyEOFFlag = 59563; 

int option;
int numConsumers = 10;
int index; //for loop counting

//create shared memory locations
shmidTurn = shmget(turnKey, SHM_SIZE, IPC_CREAT | 0666);
shmidBuffer = shmget(bufferKey, 5*(sizeof(Buffer)), IPC_CREAT | 0666);
shmidFlag = shmget(flagKey, SHM_SIZE, IPC_CREAT | 0666);
shmidProcesses = shmget(processesKey, 18*(sizeof(Process)), IPC_CREAT | 0666);

//Attach to shared memory
int * turn = (int *)shmat(shmidTurn, NULL, 0);
bufferTable = (Buffer *)shmat(shmidBuffer, NULL, 0);
state * flag = (state *)shmat(shmidFlag, NULL, 0);
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

		case 'h' : // print_usage();
			return (0);
			break;

	default : 
		printf("No number of consumers specified. Default is 10\n");
		break;	
	}
}

	printf("No processes forked yet\n");

	//assign process 0 as producer
	processes[0].producer = true;

	//assign remaining processes as consumer
	for (index = 0; index < numConsumers; index++){
		processes[index].consumer = true;
	}

	//create producer child
	childPid = fork();

	if (childPid == -1){
		printf("Failed to fork.  Aborting");
		exit(0);
	} else if (childPid == 0){
		processes[0].pid = childPid;
		processes[0].index = 0;
		execl("./producer", NULL);
	}



//create consumer children
	for (index = 1; index <= numConsumers; index++){
		//printf("in consumer creation loop\n");
		childPid = fork();
		if (childPid == -1){
			printf("failed to fork. Aborting");
			exit(0);
		} else if (childPid == 0){
			char intBuffer[3];
			sprintf(intBuffer, "%d", index);
			processes[index].pid = childPid;
			processes[index].index = index;
			execl("./consumer", "consumer", intBuffer, NULL);
		}
	}


//printf("Waiting on the kids\n");

wait(NULL);
//printf("End of master\n");


shmdt(turn);
shmdt(flag);
//shmdt(EOFflag);
shmdt(bufferTable);


shmctl(shmidTurn, IPC_RMID, NULL);
shmctl(shmidFlag, IPC_RMID, NULL);
shmctl(shmidBuffer, IPC_RMID, NULL);
//shmctl(shmidEOFFlag, IPC_RMID, NULL);

exit(1);
}



	//printf("%d", turn);

/* Code is just to check if parent was sending data to child and child could read it

pipe(fd);

if (fork() !=0) {

close (fd[0]);

write(fd[1], producerDesc, strlen(producerDesc)+1);
write(fd[1], &numConsumers, sizeof(childID));
printf("Parent(%d): There are  %d number of consumers\n", getpid(), numConsumers);
printf("Parent(%d): Child's description is %s\n", getpid(), producerDesc);
close(fd[1]);

} else {

close(fd[1]);

read(fd[0], &childID, sizeof(childID));
read(fd[0], producerDesc, strlen(producerDesc)+1);
printf("Child(%d): There are %d number of consumers\n", getpid(), numConsumers);
printf("Child(%d): I am a %s\n", getpid(), producerDesc);
close(fd[0]);
}
*/

