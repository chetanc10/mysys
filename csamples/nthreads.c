
#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#define NTHREADS 2
#define N 1000
#define MEGEXTRA 1000000

pthread_attr_t attr;
static int i;
pthread_t threads[NTHREADS];

void pf (pthread_t id)
{
	static pthread_t curID;

	printf ("curID: %d\r\n", curID = id);
}

void *dowork(void *threadid)
{
	double A[N][N];
	int i,j;
	long tid;
	size_t mystacksize;
	cpu_set_t cpuset;

	CPU_ZERO(&cpuset);
	CPU_SET(i++, &cpuset);
	pthread_setaffinity_np (pthread_self(), sizeof(cpu_set_t), &cpuset);
	tid = (long)threadid;
	pthread_attr_getstacksize (&attr, &mystacksize);
	printf("Thread %ld: stack size = %li bytes \n", tid, mystacksize);
	for (i=0; i<N; i++)
		for (j=0; j<N; j++)
			A[i][j] = ((i*j)/3.452) + (N-i);
	if (pthread_self () == threads[0]) {
		for (i=5; i; i--) {
			printf ("j=");
			scanf (" %d", &j);
			pf (pthread_self());
		}
	} else {
		pf (pthread_self());
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	size_t stacksize;
	int rc;
	long t;

	pthread_attr_init(&attr);
	pthread_attr_getstacksize (&attr, &stacksize);
	printf("Default stack size = %li\n", stacksize);
	stacksize = sizeof(double)*N*N+MEGEXTRA;
	printf("Amount of stack needed per thread = %li\n",stacksize);
	pthread_attr_setstacksize (&attr, stacksize);
	printf("Creating threads with stack size = %li bytes\n",stacksize);
	for(t=0; t<NTHREADS; t++){
		rc = pthread_create(&threads[t], &attr, dowork, (void *));
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	printf("Created %ld threads.\n", t);
	pthread_exit(NULL);
}
