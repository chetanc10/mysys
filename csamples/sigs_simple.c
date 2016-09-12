#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

int count = 1;

void sig_handler(int signo)
{
	if (signo == SIGINT) {
		printf("received SIGINT\n");
		count = 0;
	} else
		printf ("what????\r\n");
}

int main(int argc, char *argv[])
{
	struct sigaction sa;

	if (signal(SIGINT, sig_handler) == SIG_ERR) {
		printf("\ncan't catch SIGINT\n");
	}

	while (count)
		sleep(1);
	printf ("DONE!!!\r\n");
	return 0;
}
