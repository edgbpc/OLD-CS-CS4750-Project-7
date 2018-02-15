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

#define SHM_SIZE 1024

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
void handle(int sig){
	printf("Terminating program");
	exit(0);
}

int main (int argc, char *argv[]){
	

//set alarm  timer to 100 seconds.  signal invokes SIGALRM and calls handle to terminate program
alarm(100);
signal(SIGALRM, handle);

key_t key = 59566;
char *data;
int mode;

int option;
int numConsumers;

//int fd[2];
//int producerID = 0;

char producerDesc[20] = "Producer";
char consumerDesc[20] = "Consumer";
char readDesc[20];

//int childID = 1;

int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0777);

int * turn;
int * flag;


turn = (int *)shmat(shmid, NULL, 0);
flag = turn + 1;

*turn = 5;
flag[1] = 999;


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

//create the producer child
pid_t producerPid = fork();

if (producerPid == 0){
execv("./producer", NULL);
}

int index;

for (index = 1; index <= numConsumers; index++){
	pid_t consumerPid = fork();
	if (consumerPid == 0){
	execv("./consumer", NULL);
	}
}
wait(NULL);

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
	return 0;



}	
