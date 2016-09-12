#include <stdio.h>
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
#include <math.h>

#define CIRC_INCR(idx, limit) (idx = (++idx % limit))

int main(int argc, char **argv)
{
	float f;
	int n;

	f = (float)(16*1*18*10*8)/512/10;
	printf ("f: %f\n", f);
#if 0
	n = (int)f;
	printf ("n: %d\n", n);
	n += !!((f - n)*10);
#else
	n = (int)f + !!((f - (float)(int)f) * 10);
#endif
	printf (">: %d\n", n);
	return 0;
}
