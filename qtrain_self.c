#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <wait.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#include "share.h"
#define MAX_PASSENGER 100
#define MAX_PASSENGER_IN_TRAIN 5
#define MAX_TICKET 20
#define MAX_TRAIN_THREAD 5
#define MIN_START_TRAIN 10
#define NUM_TICKET_PLACE 2

void *ticket_thread(int *);
void *train_thread(int *);

// person == ticket
pthread_mutex_t gTicketMutex;	// controls people in platform 
//int gTicket[MAX_TICKET]; 		// total people in platform
int gTicketIn;					// total number of people which has got in platform
int gTicketOut;					// total number of people which has left platform
//mutex
sem_t gStartTrainSem;			// train: waits until 10 people got in platform
sem_t gInAvailableSem;			// person enters platform while platform has less than 20 people
sem_t gOutAvailableSem;			// person leaves platform

pthread_t gTicketThread[NUM_TICKET_PLACE];
pthread_t gTrainThread[MAX_TRAIN_THREAD];
int gTicketThreadID[NUM_TICKET_PLACE];
int gTrainThreadID[MAX_TRAIN_THREAD];

int main(void)
{
	int i;
	sem_init(&gStartTrainSem, 0, 0);
	sem_init(&gInAvailableSem, 0, 20);
	sem_init(&gOutAvailableSem, 0, 0);
	pthread_mutex_init(&gTicketMutex, NULL);

	for(i=0;i<MAX_TRAIN_THREAD;i++){
		gTrainThreadID[i]=i+1;
		pthread_create(&gTrainThread[i],NULL,train_thread,&gTrainThreadID[i]);
	}

	for(i=0;i<NUM_TICKET_PLACE;i++){
		gTicketThreadID[i]=i+1;
		pthread_create(&gTicketThread[i],NULL,ticket_thread,&gTicketThreadID[i]);
	}

	
	for(i=0;i<MAX_TRAIN_THREAD;i++) pthread_join(gTrainThread[i], NULL);
	for(i=0;i<NUM_TICKET_PLACE;i++) pthread_join(gTicketThread[i], NULL);
}

void *ticket_thread(int *arg)
{
	int i;
	sleep(1);
	while(1){
		pthread_mutex_lock(&gTicketMutex);
		printf("ticket : passenger %d got ticket\n",gTicketIn+1);
		gTicketIn++;
		pthread_mutex_unlock(&gTicketMutex);
		sem_post(&gOutAvailableSem);
		sem_wait(&gInAvailableSem);
		usleep(300000);
		if(gTicketIn==10)break;
	}
	for(i=0;i<3;i++) sem_post(&gStartTrainSem);
	sleep(1);
	while(gTicketIn<99){
		sem_post(&gOutAvailableSem);
		sem_wait(&gInAvailableSem);
		pthread_mutex_lock(&gTicketMutex);
		
		printf("ticket : passenger %d got ticket\n",gTicketIn+1);
		gTicketIn++;
		pthread_mutex_unlock(&gTicketMutex);

		usleep(300000);
	}

}

void *train_thread(int *arg)
{	int total=0;
	struct msgbuf msg;
	struct msgbuf recv;
	key_t key;
	int cnt,mtype,mq;

	key=ftok(MQ_KEY,0);
	mq=msgget(key,IPC_CREAT| 0);
		
	printf("======> train %d : wait\n",*arg);
	sem_wait(&gStartTrainSem);
   	printf("======> train %d : ready\n",*arg);
   	//msgrcv(mq, (void *)&recv, sizeof(struct msgbuf)-4,mtype, 0);
   	while(gTicketOut<100){
		cnt=0;
		//if(gTicketOut>=99) break;
	   	while(cnt++<5){
	   		if(gTicketOut>=100) break;
   			sem_post(&gInAvailableSem);
			sem_wait(&gOutAvailableSem);
   			pthread_mutex_lock(&gTicketMutex);
  	 	 	printf("===> train %d : passenger %d returned ticket\n",*arg,++gTicketOut);
  	 	 	pthread_mutex_unlock(&gTicketMutex);
  	 	 	total++;
  	 	 	usleep(400000);
  	 	 	
  	 	 }
	if(cnt==1)break;
	go:
	msg.mtype=*arg;
	msg.msg=MSG_REQ_RUNNING;
	msg.passengers=cnt-1;
	msgsnd(mq, (void *)&msg, sizeof(struct msgbuf)-4, 0);
	printf("======> train %d send start message \n",*arg);
	mtype=20+*arg;
	msgrcv(mq, (void *)&recv, sizeof(struct msgbuf)-4,mtype, 0);
	if(recv.msg!=MSG_ACK_RUNNING){
			printf("GOT A NOK MESSAGE1\n");
			goto go;
	}
	printf("======> train %d start with %d\n",*arg,cnt-1);
	printf("======> train %d : stop\n",*arg);
	sleep(5);
	if(gTicketOut>=99) break;
	msg.msg=MSG_REQ_READY;
	msgsnd(mq, (void *)&msg, sizeof(struct msgbuf)-4, 0);
	mtype=20+*arg;
	msgrcv(mq, (void *)&recv, sizeof(struct msgbuf)-4,mtype, 0);
	if(recv.msg==MSG_NAK_READY){
		printf("GOT A NOK MESSAGE2\n");
		break;
		}
	}
	printf("======> train %d : exit(total %d passengers)\n",*arg,total);
}



