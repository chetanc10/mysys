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

int main(int argc, char **argv)
{
	int a, b, c, d, e, n;

	n = scanf ("%d:%d:%d:%d:%d", &a, &b, &c, &d, &e);

	if (n != 5) printf ("%d????\n", n);
	else printf ("%d:%d:%d:%d:%d\n", a, b, c, d, e);

	return 0;
}
