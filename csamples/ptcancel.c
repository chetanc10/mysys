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

#define wmb()   asm volatile("sfence" ::: "memory")

#define handle_error_en(en, msg) \
	do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

typedef struct MsgBuf {
	long type;
	int  msg;
} MsgBuf;

int mqid;

static void * thread_func(void *ignored_argument)
{
	MsgBuf rBuf;
	/* Disable cancellation for a while, so that we don't
	   immediately react to a cancellation request */
#if 0
	int s;

	s = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if (s != 0)
		handle_error_en(s, "pthread_setcancelstate");

	printf("thread_func(): started; cancellation disabled\n");
	sleep(5);
	printf("thread_func(): about to enable cancellation\n");

	s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if (s != 0)
		handle_error_en(s, "pthread_setcancelstate");

	/* sleep() is a cancellation point */
#endif
	msgrcv (mqid, &rBuf, sizeof (MsgBuf), 1, 0);
	printf ("Wow! msgrcv gave me %d\n", rBuf.msg);
	sleep(100);        /* Should get canceled while we sleep */

	printf("thread_func(): not canceled!\n");
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thr;
	void *res;
	int s;
	MsgBuf sBuf = {1, 12321};

	/* Start a thread and then send it a cancellation request */
	mqid = msgget (1231, IPC_CREAT | 0666);

	s = pthread_create(&thr, NULL, &thread_func, NULL);
	if (s != 0)
		handle_error_en(s, "pthread_create");

	sleep(5);           /* Give thread a chance to do something */

	printf ("msgsnd now..\n");
	msgsnd (mqid, &sBuf, sizeof (MsgBuf), 0);
	printf ("msgsnd done..\n");

	sleep(5);           /* Give thread a chance to do something */

	printf("main(): sending cancellation request\n");
	s = pthread_cancel(thr);
	if (s != 0)
		handle_error_en(s, "pthread_cancel");

	/* Join with thread to see what its exit status was */

	s = pthread_join(thr, &res);
	if (s != 0)
		handle_error_en(s, "pthread_join");

	if (res == PTHREAD_CANCELED)
		printf("main(): thread was canceled\n");
	else
		printf("main(): thread wasn't canceled (shouldn't happen!)\n");

	msgctl (mqid, IPC_RMID, 0);
	exit(EXIT_SUCCESS);
}
