
#define _GNU_SOURCE

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#if 1
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
#include <sys/utsname.h>
#include <math.h>
#include <dirent.h>
#include <sched.h>
#include <linux/if_link.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

void segfault_handler (int signum, siginfo_t *si, void *arg)
{
	int i = 0;
	printf ("**** encountered seg-fault ****\n");
	printf (" signo       : %ld \n", (long  )(si->si_signo));
	printf (" errno       : %ld \n", (long  )(si->si_errno));
	printf (" code        : %ld \n", (long  )(si->si_code));
	printf (" status      : %ld \n", (long  )(si->si_status));
	/*printf ("sival        : %s \n", (char *)(si->si_value.sival_int));*/
	printf (" si_int      : %ld \n", (long  )(si->si_int));
	printf (" si_ptr      : %p \n", (char *)(si->si_ptr));
	/*printf ("             : %ld \n", (long  )(si->si_));*/
	printf (" killer pid  : %ld \n", (long  )(si->si_pid));
	printf (" faulty-addr : %p \n", (char *)(si->si_addr));
#if 1
	while (1) {
		printf ("\r%d", i++);
		sleep (1);
	}
#endif
}

char progname[32] = "";

void f2 (char *buf)
{
	char lbuf[32] = "CtaN";
	/*memcpy (NULL, lbuf, 8);*/
	sleep (2);
	if (lbuf[3]) {
		printf ("%d\n", 1/0);
	}
}

void f3 (char *buf)
{
	printf ("f3-buf: %s\n", buf);
}

void f1 (void *sa)
{
	if (sa) {
		printf ("calling f2()\n");
		f2 ((char *)&f2);
	} else {
		printf ("calling f3()\n");
		f2 ("f3()");
	}
}

int main (int argc, char **argv)
{
	struct sigaction sa;
	struct sigevent se;

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = &segfault_handler;
	sigemptyset (&sa.sa_mask);
	sigaction (SIGSEGV, &sa, NULL);
	sigaction (SIGFPE, &sa, NULL);

	memcpy (progname, argv[0], strlen (argv[0]));
	/*se.sigev_notify = SIGEV_SIGNAL;*/
	/*se.sigev_signo = SIGSEGV;*/
	/*se.sigev_value.sival_ptr = progname;*/

	printf ("Setup done..\n");
	f1 (&sa);

	while (1) {
		sleep (1);
	}

	return 0;
}
