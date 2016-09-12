
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
#include <sys/utsname.h>
#include <math.h>

int main (void)
{
	struct utsname uts = {0};

	if (uname (&uts) < 0) {
		return -1;
	}

	printf ("sysname: %s\n", uts.sysname);
	printf ("nodename: %s\n", uts.nodename);
	printf ("release: %s\n", uts.release);
	printf ("version: %s\n", uts.version);
	printf ("machine: %s\n", uts.machine);

	return 0;
}

