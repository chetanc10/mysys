
#define _GNU_SOURCE
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

/* For error handling */
#include <stdlib.h>
#include <errno.h>
#include <error.h>

static volatile int event_fd;

#define NSIGMAX (15 + SIGRTMIN)

static void handler (int sig, siginfo_t *si, void *data)
{
	event_fd = si->si_fd;
}

int main (int argc, char *argv[])
{
	struct sigaction sa;
	int fd = 0;
	int sn = SIGRTMIN + 1;

	if (argc < 2)
		error (EXIT_FAILURE, 0, "Usage: ./dnotify <absolute-path-of-directory>");

	sa.sa_sigaction = handler;
	sigemptyset (&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	/*while ((sn < NSIGMAX) && sigaction (sn, &sa, NULL)) sn++;*/
	sigaction (sn, &sa, NULL);

	printf ("sn: %d\n", sn);
	if (sn == NSIGMAX) errx (EXIT_FAILURE, "Unable to get a free SIGXXX..");

	if ((fd = open (argv[1], O_RDONLY)) < 0)
		error (EXIT_FAILURE, errno, "failed to open '%s'", argv[1]);

	if (fcntl (fd, F_SETSIG, sn) < 0)
		error (EXIT_FAILURE, errno, "failed to set dnotify signal");

	if (fcntl (fd, F_NOTIFY, DN_MODIFY|DN_CREATE|DN_MULTISHOT))
		error (EXIT_FAILURE, errno, 
				"failed to register notification for '%s'", argv[1]);

	while (1) {
		pause ();
		printf ("event occured for fd=%d\n", event_fd);
	}
}

