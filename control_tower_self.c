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
int main(void){
	struct msgbuf msg;
	struct msgbuf recv;
	key_t key;
	int cnt,mtype,mq;
	
	key=ftok(MQ_KEY,0);
	if((mq=msgget(key,0))==-1) {
		printf("CT : MQ CANT BE CREATED\n");
	 	return -1; 
	}	
	else {
		printf("CT : message queue created (myqid = %x , key = %d) \n",mq,key);
	}
	while(1){
		usleep(1000000);
		msgrcv(mq, (void *)&recv, sizeof(struct msgbuf)-4,0,0);
		printf("****%ld\n",recv.mtype%10);
		switch(recv.mtype%10){
			case 1: if(recv.msg==MSG_REQ_RUNNING){
							printf("Got MSG_REQ_RUNNING form train 1\n");
							msg.mtype=21;
							msg.msg=MSG_ACK_RUNNING;
							msgsnd(mq, (void *)&msg, sizeof(struct msgbuf)-4, 0);
							printf("Send MSG_ACK_RUNNING from train1\n");
							printf("CT : %d passenger in train 1\n",recv.passengers);
						}
						else{
							printf("Got MSG_REQ_READY form train 1\n");
							msg.mtype=21;
							msg.msg=MSG_ACK_READY;
							msgsnd(mq, (void *)&msg, sizeof(struct msgbuf)-4, 0);
							printf("Send MSG_ACK_READY from train1\n");
						}
						break;
			case 2: if(recv.msg==MSG_REQ_RUNNING){
							printf("Got MSG_REQ_RUNNING form train 2\n");
							msg.mtype=22;
							msg.msg=MSG_ACK_RUNNING;
							msgsnd(mq, (void *)&msg, sizeof(struct msgbuf)-4, 0);
							printf("Send MSG_ACK_RUNNING from train2\n");
							printf("CT : %d passenger in train 2\n",recv.passengers);
						}
						else{
							printf("Got MSG_REQ_READY form train 2\n");
							msg.mtype=22;
							msg.msg=MSG_ACK_READY;
							msgsnd(mq, (void *)&msg, sizeof(struct msgbuf)-4, 0);
							printf("Send MSG_ACK_READY from train2\n");
						}
						break;

			case 3: if(recv.msg==MSG_REQ_RUNNING){
							printf("Got MSG_REQ_RUNNING form train 3\n");
							msg.mtype=23;
							msg.msg=MSG_ACK_RUNNING;
							msgsnd(mq, (void *)&msg, sizeof(struct msgbuf)-4, 0);
							printf("Send MSG_ACK_RUNNING from train3\n");
							printf("CT : %d passenger in train 3\n",recv.passengers);
						}
						else{
							printf("Got MSG_REQ_READY form train 3\n");
							msg.mtype=23;
							msg.msg=MSG_ACK_READY;
							msgsnd(mq, (void *)&msg, sizeof(struct msgbuf)-4, 0);
							printf("Send MSG_ACK_READY from train3\n");
						}
						break;

			case 4: if(recv.msg==MSG_REQ_RUNNING){
							printf("Got MSG_REQ_RUNNING form train 4\n");
							msg.mtype=24;
							msg.msg=MSG_ACK_RUNNING;
							msgsnd(mq, (void *)&msg, sizeof(struct msgbuf)-4, 0);
							printf("Send MSG_ACK_RUNNING from train4\n");
							printf("CT : %d passenger in train 4\n",recv.passengers);
						}
						else{
							printf("Got MSG_REQ_READY form train 4\n");
							msg.mtype=24;
							msg.msg=MSG_ACK_READY;
							msgsnd(mq, (void *)&msg, sizeof(struct msgbuf)-4, 0);
							printf("Send MSG_ACK_READY from train4\n");
						}
						break;

			case 5: if(recv.msg==MSG_REQ_RUNNING){
							printf("Got MSG_REQ_RUNNING form train 5\n");
							msg.mtype=25;
							msg.msg=MSG_ACK_RUNNING;
							msgsnd(mq, (void *)&msg, sizeof(struct msgbuf)-4, 0);
							printf("Send MSG_ACK_RUNNING from train5\n");
							printf("CT : %d passenger in train 5\n",recv.passengers);
						}
						else{
							printf("Got MSG_REQ_READY form train 5\n");
							msg.mtype=25;
							msg.msg=MSG_ACK_READY;
							msgsnd(mq, (void *)&msg, sizeof(struct msgbuf)-4, 0);
							printf("Send MSG_ACK_READY from train5\n");
						}
						break;

			default : break;
		}

	}
}
