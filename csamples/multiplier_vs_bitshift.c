
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

#define MIN(a,b) ((a<b)?a:b)

#define BITS_SHIFT(n) (log2 (n))

int main (int argc, char **argv)
{
	int n = 1;
	int m;

	scanf ("%d", &m);

	printf ("::%lu\n", MIN (m, n) * sizeof (uint64_t));
	printf ("::%lu\n", MIN (m, n) << (int)BITS_SHIFT (sizeof (uint64_t)));
	return 0;
}

