#define RETRY 100000000
#define MQ_KEY	"/bin/login"

#define MAX_TRAIN 20

struct msgbuf {
	long mtype;
	int msg;
	int passengers;
};
#define MSIZE (sizeof(struct msgbuf)-sizeof(long)) 

#define MTYPE_ALL	0

#define MSG_REQ_READY	1
#define MSG_ACK_READY	2
#define MSG_NAK_READY	3
#define MSG_REQ_RUNNING	4
#define MSG_ACK_RUNNING	5

