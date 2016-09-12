
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

#define func(a, b, c, d) \
{ \
	if (a >= b) { \
#if CONSIDERING_C \
		if (a >= c) d = a; \
		else d = c; \
#else \
		d = a; \
#endif \
	} else { \
#if CONSIDERING_C \
		if (b >= c) d = b; \
		else d = c; \
#else \
		d = b \
#endif \
	} \
}

int main (int argc, char **argv)
{
	int fd = 0;
	int ret = 0;
	char buf[8] = "";
	char *ptr = buf;
	fd_set readfds;
	int cnt;
	struct timeval tv = {1, 0};

	FD_ZERO (&readfds);

	FD_SET (0, &readfds);
	while (1) {
		ret = select (1, &readfds, NULL, NULL, &tv);
		if (ret < 0) {
			tv.tv_sec = 2;
			tv.tv_usec = 0;
			printf ("tout!\n");
			FD_SET (0, &readfds);
		} else if (ret == 0) {
			FD_SET (0, &readfds);
		} else if (FD_ISSET (0, &readfds)) {
			int n = read (0, ptr, 1);
			printf ("read: %c\n", *ptr);
			ptr += n;
			if (cnt++ == 5) break;
			/*FD_SET (0, &readfds);*/
		} else {
			printf (":");
			/*FD_SET (0, &readfds);*/
		}
	}
	printf ("%s\n", buf);
	return 0;
}
