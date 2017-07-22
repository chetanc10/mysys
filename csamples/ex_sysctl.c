
#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
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
#include <sys/resource.h>
#include <dirent.h>
#include <emmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <linux/sysctl.h>

int main(int argc, char **argv)
{
	int name[] = {CTL_KERN, KERN_PRINTK};
	int namelen = 2;
	int oldval[4];  /* 4 would suffice */
	size_t len = sizeof(oldval);
	int i, error;

	if (argc != 2) {
		printf ("Usage: ./a.out debug-log-level (4-8)\n");
		return -1;
	}

	error = sysctl (name, namelen,
			(void *)oldval, &len, NULL
			/* newval */, 0 /* newlen */);
	if (error) {
		fprintf(stderr,"%s: sysctl(): %s\n",
				argv[0],strerror(errno));
		exit(1);
	}

	printf("len is %i bytes\n", (int)len);
	for (i = 0; i < len/(sizeof(int)); i++)
		printf("%i\t", oldval[i]);
	printf("\n");

	oldval[0] = atoi (argv[1]);

	error = sysctl (name, namelen,
			NULL, NULL, (void *)oldval, len);
	if (error) {
		fprintf(stderr,"%s: sysctl(): %s\n",
				argv[0],strerror(errno));
		exit(1);
	}
	exit(0);
}
