#ifndef OSS_HEADER_FILE
#define OSS_HEADER_FILE


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <sys/msg.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>
#include <semaphore.h>

//macro definations

#define SHM_SIZE 2000
#define BILLION 1000000000L //taken from book


//governs maximum amouut of processes and resources.  change to increase/decrease
#define maxProcesses 18
#define maxResources 20
#define BUFFERSIZE 50
#define empty 0
#define full 1

//structures

typedef enum { 
	idle, want_in, in_cs
} state;

typedef struct {
	char data[BUFFERSIZE];
	bool isFull;
}Buffer;

typedef struct {
	bool producer;
	bool consumer;
	int pid;
	int index;
} Process;

typedef struct
{
    int front, rear, size;
    unsigned capacity;
    int* array;
    int RTLocation;
    int pid;
} Queue;

//prototypes
void handle(int signo);
void terminateSharedResources();
static int setperiodic(double sec);
void calculateTime();

//Queue Prototypes
Queue* createQueue(unsigned capacity);
int isFull(Queue* queue);
int isEmpty(Queue* queue);
void enqueue(Queue* queue, int item);
int dequeue(Queue* queue);
int front(Queue* queue);
int rear(Queue* queue);

int shmidTurn;
int shmidBuffer;
int shmidFlag;
int shmidProcesses;

//Shared Memory Keys
key_t turnKey;
key_t bufferKey;
key_t flagKey;
key_t processesKey;

#endif

