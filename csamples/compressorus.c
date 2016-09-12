
#include <stdio.h>

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
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
#include <sys/syscall.h>
#include <math.h>

#define MIN(a,b) (a<b)?a:b

int main (int argc, char **argv)
{
	uint8_t *p = NULL;
	int fd;
	int i;
	int temp;
	float fsz = (UINT32_MAX + 1UL)/4 + (float)54*1024*1024*1024/100;
	uint64_t sz = (uint64_t)fsz;
	uint64_t nb;
	char ch;
	printf ("sz: %u\n", sz);
	p = malloc (sz - 0);
	if (!p) {
		perror ("p");
		return -1;
	}

	fd = open ("./p", O_RDWR | O_CREAT);
	if (fd < 0) {
		perror ("fd");
		free (p);
		return -2;
	}

	printf ("want to randomize? (y/n): ");
	scanf (" %c", &ch);
	if (ch == 'y') {
		for (i = 0; i < sz/8; i++) {
			temp *= 17;
			temp %= 1427;
			*((uint64_t *)p + i) = (uint64_t) (i % 18 + temp);
		}
	}

	printf ("writing..\n");
	nb = write (fd, p, sz);
	if (nb != sz) {
		perror ("write-fd");
		printf ("%u:%u\n", sz, nb);
		free (p);
		return -3;
	}
	printf ("done..\n");

	/*if (system ("tar -cf ")) {*/
	/*}*/
	printf ("success\n");
	free (p);
	return 0;
}

