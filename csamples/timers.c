#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define CLOCKID CLOCK_REALTIME
#define SIG SIGALRM
static timer_t timerid;

uint64_t count = 1;

int r = 1;
static void timeout_handler(int sig, siginfo_t *si, void *uc)
{
	/*if(si->si_value.sival_ptr != &count){*/
	/*printf ("strayso: %p: %d\n", si->si_value.sival_ptr, *(int *)(si->si_value.sival_ptr));*/
	/*} else {*/
	printf ("counto: %p: %p\n", si->si_value.sival_ptr, *(uint64_t *)(si->si_value.sival_ptr));
	r = 0;
	/*}*/
}

int main(int argc, char *argv[])
{
	struct sigevent sev;
	struct itimerspec its = {0};
	struct sigaction sa;
	printf("Establishing handler for signal %d\n", SIG);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = timeout_handler;
	sigemptyset(&sa.sa_mask);
	sigaction(SIG, &sa, NULL);

	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIG;
	sev.sigev_value.sival_ptr = &count;
#if 0
	if (timer_delete (timerid)) {
		perror("timer_delete:");
		return errno;
	}
#endif
	timer_create(CLOCKID, &sev, &timerid);
	count = (uint64_t)timerid;
	printf ("timer: %p\n", timerid);
	printf ("count: %p\n", count);

	/* Start the timer */
	its.it_value.tv_sec = 4;
	its.it_value.tv_nsec = 0;
	//     its.it_interval.tv_sec = its.it_value.tv_sec;
	//     its.it_interval.tv_nsec = its.it_value.tv_nsec;

	timer_settime(timerid, 0, &its, NULL);
	while (r)
		sleep(3);
	printf ("done with timers: ");
	printf ("%d\n", timer_delete (timerid));
	return 0;
}
