
/**
 * Compilation: gcc -DUSLEEP_TOUT=<0 or +ve integer> ex_sys_v_msgq.c -pthread
 */

#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <pthread.h>

#if (defined (USLEEP_TOUT)) && (USLEEP_TOUT)
#define USLEEP(n) usleep (n)
#else
#ifndef USLEEP_TOUT
#define USLEEP_TOUT 0
#endif
#define USLEEP(n)
#endif

uint32_t mqid;

typedef struct MsgBuf {
	long type;
	int msg;
} MsgBuf;

void *ctf (void *arg)
{
	MsgBuf rBuf;

	while (1) {
		if (msgrcv (mqid, &rBuf, sizeof (MsgBuf), 1, IPC_NOWAIT) > 0) {
			printf ("received: %c\n", (char)rBuf.msg);
			if (((char)rBuf.msg == 'q') || ((char)rBuf.msg == 'x')) {
				printf ("exiting ctf!\n");
				pthread_exit (0);
			}
		}
		USLEEP (USLEEP_TOUT);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t ctid;
	MsgBuf sBuf = {1, 0};
	char ch = 0;
	mqid = msgget (1234, IPC_CREAT|0666);
	if (mqid<0) {
		perror("mqid");
		return -1;
	}
	pthread_create (&ctid, NULL, ctf, NULL);
	while ((ch != 'x') && (ch != 'q')) {
		scanf (" %c", &ch);
		sBuf.msg = ch;
		msgsnd (mqid, &sBuf, sizeof (MsgBuf), 1);
	}
	pthread_join (ctid, NULL);
	msgctl (mqid, IPC_RMID, 0);
	printf("All done\n");
	return 0;
}

