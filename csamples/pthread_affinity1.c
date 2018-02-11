#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>

struct foo {
	char a:4;
	char c:4;
	// 	char d:4;
	char b:8;
} __attribute__ ((packed));

void affinity(int cpuid, pthread_t thread)
{
	cpu_set_t cpuset1, cpuset2;
	int ret_val;

	CPU_ZERO(&cpuset1); /* Initializing the CPU set to be the empty set */
	CPU_ZERO(&cpuset2); /* Initializing the CPU set to be the empty set */
	CPU_SET(cpuid , &cpuset1);  /* setting CPU on cpuset */

	ret_val = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset1);
	if(ret_val != 0) {
		perror("pthread_setaffinity_np");
	}
	ret_val = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset2);
	if(ret_val != 0) {
		perror("pthread_getaffinity_np");
	} else if (!CPU_EQUAL (&cpuset1, &cpuset2)) {
		perror("affinity improper\r\n");
	}
}

void *processor_thread (void *arg)
{
	int m = *(int *)arg;
	printf ("t1: %lu\n", pthread_self());
	while (m--) {
		printf ("t1 ");
		sleep (1);
	}

	return NULL;
}

void *io_thread (void *arg)
{
	int m = *(int *)arg;
	printf ("t2: %lu\n", pthread_self());
	while (m--) {
		printf ("t2 ");
		sleep (1);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	int n = 20, ret;
	printf ("%u\n", (unsigned int)sizeof(int));

	pthread_t process_poll, io_poll;
	if((ret = pthread_create(&process_poll, NULL, processor_thread, &n)) != 0) {
		perror("pthread_create");
		return -1;
	}
	affinity(0, process_poll);
	if((ret = pthread_create(&io_poll, NULL, io_thread, &n)) != 0) {
		perror("pthread_create");
		return -1;
	}
	affinity(1, io_poll);

	pthread_join(process_poll, NULL);
	pthread_join(io_poll, NULL);

	return 0;
}
