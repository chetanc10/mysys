#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <pthread.h>
#include <semaphore.h>
#include <limits.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define bdbm_bug_on(condition)  \
	do {    \
		if (condition)  { \
			bdbm_error ("bdbm_bug_on"); \
			while(1) sleep(1); \
		} \
	} while (0);


#define wmb()   asm volatile("sfence" ::: "memory")

uint32_t mqid;
volatile int flag = 0;

typedef struct MsgBuf {
	long type;
	int msg;
} MsgBuf;

void  ALARMhandler(int sig)
{
	/*signal(SIGALRM, SIG_IGN);          *//* ignore this signal       */
	printf ("flagged!\n");
	flag++;
}

void *ctf (void *arg)
{
	MsgBuf rBuf;

	signal(SIGALRM, ALARMhandler);     /* install the handler    */
	alarm(3);
	while (1) {
		if (msgrcv (mqid, &rBuf, sizeof (MsgBuf), 1, IPC_NOWAIT) < 0) {
			/*timeout!!*/
			if (flag) {
				flag &= 0;
				signal(SIGALRM, ALARMhandler);     /* reinstall the handler    */
				alarm(3);
			}
		} else {
			printf ("::%d\n", rBuf.msg);
		}
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	signal(SIGALRM, ALARMhandler);     /* reinstall the handler    */
	alarm(3);
	pthread_t ctid;
	MsgBuf sBuf = {1, 0};
	char ch;
	mqid = msgget (1234, IPC_CREAT|0666);
	if (mqid<0) {
		perror("mqid");
		return -1;
	}
	pthread_create (&ctid, NULL, ctf, NULL);
	while (1) {
		scanf (" %c", &ch);
		sBuf.msg = ch+1;
		msgsnd (mqid, &sBuf, sizeof (MsgBuf), 1);
	}
	pthread_join (ctid, NULL);
	msgctl (mqid, IPC_RMID, 0);
	printf("All done");
	return 0;
}
