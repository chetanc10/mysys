
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include <utmp.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <execinfo.h>
#include <signal.h>
#include <syslog.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>

va_list work;

void subby (char *name)
{
	if (!strcmp ("c10", name)) {
		printf ("c10: %d, %f\n", va_arg (work, int), va_arg (work, double));
	} else {
		printf ("c10: %f, %s\n", va_arg (work, double), va_arg (work, char *));
	}
}

int print_mydb (char *name,...)
{
	va_list ap;

	va_start (ap, name);         /* Initialize the argument list. */
	va_copy (work, ap);
	/*subby (name);*/
	va_end (ap);                  /* Clean up. */

	return 0;
}

int main (void)
{
	/* This call prints 16. */
	print_mydb ("c10", 3, 5.23);
	subby ("c10");
	print_mydb ("c11", -124.43, "database?!");
	subby ("c11");

	return 0;
}
