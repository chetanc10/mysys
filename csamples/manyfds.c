
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

int main (int argc, char **argv)
{
	int fd[8] = {0};
	int i = 0;

	for (i = 0; i < 8; i++) {
		fd[i] = open ("/dev/mem", O_RDWR);
		printf ("%s: fd[%d] = %d\n", strerror (errno), i, fd[i]);
	}

	for (i = 0; i < 8; i++) {
		close (fd[i]);
	}
	return 0;
}
