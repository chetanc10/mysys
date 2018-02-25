#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

/* compile with gcc -pthread lockwait.c */

#define MAX_THREADS 4

pthread_cond_t cv;
pthread_mutex_t lock;
pthread_t t[MAX_THREADS];

void *thread(void *v) {
	int tid = ((pthread_t *)v) - t;
	printf("Thread running: %d\n", ((pthread_t *)v) - t);

	while (1) {
		sleep (tid); /* might have been different work loads for different works */
		pthread_mutex_lock(&lock);
		pthread_cond_wait(&cv, &lock);
		printf("%d unlocked\n", tid);
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

int main()
{
	char cmd[1024];
	int i = 0;

	pthread_mutex_init (&lock, NULL);
	pthread_cond_init (&cv, NULL);

	for (i = 0; i < MAX_THREADS; i++) {
		pthread_create (&t[i], NULL, thread, &t[i]);
	}

	printf ("Type s to signal, b to broadcast, x to exit\n");

	printf ("cond > ");
	while(fscanf(stdin, "%s", cmd) != EOF) {
		switch (cmd[0]) {
			case 's':
				pthread_cond_signal (&cv);
				break;
			case 'b':
				pthread_cond_broadcast (&cv);
				break;
			case 'x':
				goto EXIT;
		}
		sleep (1);
		printf ("\ncond > ");
	}

EXIT:

	for (i = 0; i < MAX_THREADS; i++) {
		pthread_kill (t[i], SIGKILL);
		pthread_join (t[i], NULL);
	}

	pthread_cond_destroy (&cv);
	pthread_mutex_destroy (&lock);
	return 1;
}
