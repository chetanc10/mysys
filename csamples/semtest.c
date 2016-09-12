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

#define wmb()   asm volatile("sfence" ::: "memory")

typedef struct {
	sem_t s;
} csem_t;

csem_t cs;

#define LOOPER 1

void *child(void *arg)
{
	// signal here: child is done
#if LOOPER
	while (1) {
		printf("child\n");
		sleep (2);
		sem_post(&cs.s);
		printf ("posted to parent\n");
		sleep (1);
		sem_wait(&cs.s);
		printf ("posted by parent\n");
	}
#else
	sleep (1);
	sem_post (&cs.s);
#endif
	return NULL;
}

int main(int argc, char *argv[])
{
	sem_init(&cs.s, 0, 0); // what should X be?
	pthread_t c;
	pthread_create(&c, NULL, child, NULL);
	// wait here for child
#if LOOPER
	while (1) {
		sem_wait(&cs.s);
		sleep (1);
		sem_post(&cs.s);
		printf ("posted in parent\n");
	}
#else
	/*sem_wait (&cs.s);*/
	printf ("posted for parent\n");
	printf ("::%d::%d\n", sem_trywait (&cs.s), errno);
	sleep (1);
	printf ("::%d::%d\n", sem_trywait (&cs.s), errno);
#endif
	return 0;
}
