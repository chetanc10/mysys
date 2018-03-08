#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/prctl.h>

static char env_mac[32] = "34:de:f8:e7:2b:9b";

void *tester (void *arg)
{
	char myname[16];
	unsigned char macbytes[6];

	sscanf (env_mac, "%02x:%02x:%02x:%02x:%02x:%02x", \
			&macbytes[0], &macbytes[1], &macbytes[2], &macbytes[3], &macbytes[4], &macbytes[5]);

	sprintf (myname, "%s%02x%02x%02x%02x%02x%02x", \
			(char *)arg, macbytes[0], macbytes[1], macbytes[2], macbytes[3], macbytes[4], macbytes[5]);

	prctl (PR_SET_NAME, myname, 0, 0, 0);
	perror ("prctl");
	printf ("sleeping child\n");

	while (1) {
		sleep (1);
	}
}

int main (int argc, char **argv)
{
	pthread_t tid;
	char ch;

	char *myname = (char *)"cia_hacker";
	prctl (PR_SET_NAME, myname, 0, 0, 0);
	perror ("prctl");

	pthread_create (&tid, NULL, tester, "ch0007");

	sleep (1);
	printf ("enter x to kill the child: ");
	scanf (" %c", &ch);

	pthread_cancel (tid);
	pthread_join (tid, NULL);

	printf ("enter x to kill the hacker: ");
	scanf (" %c", &ch);

	return 0;
}

