#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define TOUT 3

timer_t timerID;
struct sigevent   sev;
struct itimerspec its;
struct sigaction  sa;

struct dummy {
	int seconds_elapsed;
	char name[12];
};

struct dummy dummy = {0, "dummy"};

#define SEC_TO_NANOSEC(n) (n * 1000000000)

void dummy_cb (int sig, siginfo_t *si, void *uc)
{
	struct dummy *d = si->si_value.sival_ptr;

	system ("date");

	if (++d->seconds_elapsed >= TOUT) {
		/*memset (&its, 0, sizeof (its));*/
		/*timer_settime (timerID, 0, &its, NULL);*/
		/*perror ("timer_settime");*/
		printf ("DONE!!!!\n");
	}
}

void create_timer (void *timerCB, void *arg, uint64_t expiry_ns, int signo)
{
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = timerCB;
	sigemptyset (&sa.sa_mask);
	sigaction (signo, &sa, NULL);
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = signo;
	sev.sigev_value.sival_ptr = arg;

	its.it_value.tv_sec = expiry_ns / 1000000000;
	its.it_value.tv_nsec = expiry_ns % 1000000000;
	its.it_interval.tv_sec = expiry_ns / 1000000000;
	its.it_interval.tv_nsec = expiry_ns % 1000000000;

	if (timer_create (CLOCK_REALTIME, &sev, &timerID)) {
		perror ("timerALARM");
		exit (-1);
	}

	if (timer_settime (timerID, 0, &its, NULL)) {
		perror ("timer_settime:");
		exit (-2);
	}
}

int main (int argc, char **argv)
{
	create_timer (dummy_cb, &dummy, SEC_TO_NANOSEC (1), SIGALRM);
	while (dummy.seconds_elapsed < TOUT) {
		printf ("::%d\n", dummy.seconds_elapsed);
		usleep (500000);
	}
	/*memset (&its, 0, sizeof (its));*/
	sigprocmask(SIG_UNBLOCK, &sa.sa_mask, NULL);
	/*timer_settime (timerID, 0, &its, NULL);*/
	/*perror ("timer_settime");*/
	timer_delete (timerID);
	/*perror ("timer-delete");*/
	return 0;
}

