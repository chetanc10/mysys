
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

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

#include "uatomic.h"

atomic_t n = ATOMIC_INIT (2);

int main (int argc, char **argv)
{
	int m = 3;

	while (1) {
		if (unlikely (m == 1)) {
			atomic_set (&n, 3);
			printf ("\rbefore: %d ", atomic_read (&n));
			atomic_inc (&n);
			printf ("after : %d", atomic_read (&n));
		} else {
			return -1;
		}
	}
	return 0;
}
