
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

typedef struct minor1 {
	int cnt;
	char *ptr;
} minor1_t;

typedef struct minor2 {
	int cnt1;
	uint8_t *ptr;
} minor2_t;

typedef struct major {
	int n;
	int bv;
	union {
		minor1_t minor1;
		minor2_t minor2;
	};
} major_t;

int main (int argc, char **argv)
{
	major_t m = {0};
	m.minor1.cnt = 12321;
	printf (": %d,%d\n", m.minor1.cnt, m.minor2.cnt1);
	return 0;
}
