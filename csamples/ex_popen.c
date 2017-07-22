
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

int main (int argc, char **argv)
{
	char display_buf[10240] = "";
	FILE *stream = NULL;
	int cnt = 0;

	stream = popen("date +'%a %b %e %Y %H:%M:%S %Z'", "r");
	if (stream == NULL)
	{
		sprintf(display_buf, "Fri Dec 31 1999 12:00:00 PST");
		printf ("\ndefaulted to: %s\n", display_buf);
		return 0;
	}

	while (fgets(display_buf, sizeof(display_buf), stream) != NULL)
	{
		cnt++;
	}

	printf ("\nfinal result after %d iterations: %s\n", cnt, display_buf);

	pclose(stream);

	return 0;
}

