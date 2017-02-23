
#define _GNU_SOURCE

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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
#include <dirent.h>
#include <sched.h>

int flag;

void f4 (void);
void f3 (void);
void f2 (void);
void f1 (void);

typedef struct {
	int n1;
	uint32_t n2;
} s_t;

void f5 (void)
{
	printf ("f5 ");
	flag = 5;
	f4 ();
}

void f4 (void)
{
	printf ("f4 ");
	if (flag == 3) {
		flag = 4;
		f5 ();
	} else {
		f3 ();
	}
}

void f3 (void)
{
	printf ("f3 ");
	if (flag == 2) {
		flag  = 3;
		f4 ();
	} else {
		f2 ();
	}
}

void f2 (void)
{
	printf ("f2 ");
	if (flag == 1) {
		flag = 2;
		f3 ();
	} else {
		f1 ();
	}
}

void f1 (void)
{
	printf ("f1 ");
	if (!flag) {
		flag = 1;
		f2 ();
	}
}

int main (int argc, char **argv)
{
	uint32_t cnt = 1;
	s_t *s = NULL;
	void (*func) (void);

#ifdef SKIP_F1
	func = f5;
#else
	func = f1;
#endif

	s = malloc (sizeof (*s));

	while (cnt--)
#if 0
	if (s != NULL) memset (s, 0, sizeof (*s));
#else
	memset (s, 0, !!(uint64_t)s * sizeof (*s)), printf ("Sleep iter: %u\n", cnt), func (), sleep (1);
#endif

	free (s);

	return 0;
}
