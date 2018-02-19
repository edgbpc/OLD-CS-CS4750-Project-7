//Author: Eric Goodwin
//Project 2 - Current Unix Processes
//Class: OS, Evening section
//
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

#define SHM_SIZE 1000
#define BUFFERSIZE 50

/*
void print_usage(){
printf("stuff");
}

int spawnProcesses(const char * program, char ** arg_list){
	pid_t child_pid = fork();
	if (child_pid != 0) {
		return child_pid; //[arent process pid
	} else {
		execvp (program, arg_list);
		fprintf (stderr, "An error occured in execvp\n");
		abort();
	}
}
*/

typedef struct {
	char buffer[BUFFERSIZE];
	int flag;
}bufferType;

typedef enum { 
	idle, want_in, in_cs
	} state;


void handle(int sig){
	printf("Terminating program");
	exit(0);
}

int main (int argc, char *argv[]){


//set alarm  timer to 100 seconds.  signal invokes SIGALRM and calls handle to terminate program
//alarm(100);
//signal(SIGALRM, handle);

key_t keyTurn = 59566;

key_t keyBufferOne = 59567;
key_t keyBufferTwo = 59568;
key_t keyBufferThree = 59569;
key_t keyBufferFour = 59560;
key_t keyBufferFive = 59561;

key_t keyFlag = 59562;
key_t keyEOFFlag = 59563; 

int option;
int numConsumers;

int status = 0;


printf("start of program\n");

//Create buffers
bufferType bufferOne;
bufferType *bufferOnePtr;

bufferType bufferTwo;
bufferType *bufferTwoPtr;

bufferType bufferThree;
bufferType *bufferThreePtr;

bufferType bufferFour;
bufferType *bufferFourPtr;

bufferType bufferFive;
bufferType *bufferFivePtr;



//create shared memory locations
int shmidTurn = shmget(keyTurn, SHM_SIZE, IPC_CREAT | 0777);
int shmidFlag = shmget(keyFlag, SHM_SIZE, IPC_CREAT | 0777);
int shmidBufferOne = shmget(keyBufferOne, SHM_SIZE, IPC_CREAT | 0777);
int shmidBufferTwo = shmget(keyBufferTwo, SHM_SIZE, IPC_CREAT | 0777);
int shmidBufferThree = shmget(keyBufferThree, SHM_SIZE, IPC_CREAT | 0777);
int shmidBufferFour = shmget(keyBufferFour, SHM_SIZE, IPC_CREAT | 0777);
int shmidBufferFive = shmget(keyBufferFive, SHM_SIZE, IPC_CREAT | 0777);
int shmidEOFFlag = shmget(keyEOFFlag, SHM_SIZE, IPC_CREAT | 0777);


//Attach to shared memory
int * turn = (int *)shmat(shmidTurn, NULL, 0);
state * flag = (state *)shmat(shmidFlag, NULL, 0);
int * EOFflag = (int *)shmat(shmidEOFFlag, NULL, 0);

*EOFflag = 1;

bufferOnePtr = (bufferType *)shmat(shmidBufferOne, NULL, 0);
bufferTwoPtr = (bufferType *)shmat(shmidBufferTwo, NULL, 0);
bufferThreePtr = (bufferType *)shmat(shmidBufferThree, NULL, 0);
bufferFourPtr = (bufferType *)shmat(shmidBufferFour, NULL, 0);
bufferFivePtr = (bufferType *)shmat(shmidBufferFive, NULL, 0);

while ((option = getopt(argc, argv, "n:h")) != -1){
	switch (option){
		case 'n' : numConsumers = atoi(optarg);
			break;

		case 'h' : // print_usage();
			return (0);
			break;

	default : //print_usage();
		exit(EXIT_FAILURE);
	}
}

printf("No processes forked yet\n");

//create the producer child
pid_t producerPid = fork();

if (producerPid == 0){
printf("Producer Process\n");
execv("./producer", NULL);
}

int index;


//create consumer children
for (index = 1; index <= numConsumers; index++){
	pid_t consumerPid = fork();
	if (consumerPid == 0){
	printf("Consumer Process\n");
	char arg1[2];
	snprintf(arg1, sizeof(arg1), "%d", index);
	execlp("./consumer", arg1, NULL );
	}
}

pid_t wpid;


printf("Waiting on the kids\n");

while((wpid = wait(&status)) > 0);;
printf("End of master\n");

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

