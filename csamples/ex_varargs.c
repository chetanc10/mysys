
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

#define THREADY 1

#if THREADY
#define MYVARP 1
#else
#define MYVARP 0
#endif

va_list vobj;

uint64_t varp[6];

void *tsub (void *arg)
{
#if MYVARP
	printf ("mvobj: %d, %f\n", (int)varp[0], (double)varp[1]);
#else
	printf ("vobj: %d, %f\n", va_arg (vobj, int), va_arg (vobj, double));
#endif
	return NULL;
}

void print_mydb (char *name, ...)
{
	va_list ap;
	va_start (ap, name);
#if MYVARP
	printf ("pap: %d, %f\n", va_arg (ap, int), va_arg (ap, double));
	va_end (ap);
	va_start (ap, name);
	varp[0] = (uint64_t)va_arg (ap, int);//uint64_t);
	varp[1] = (uint64_t)va_arg (ap, double);//uint64_t);
#else
	va_copy (vobj, ap);
#endif
	/*printf ("actual var_arg: %d, %f\n", va_arg (ap, int), va_arg (ap, double));*/
	va_end (ap);
}

int main (void)
{
	pthread_t tid;

	print_mydb ("b10", 6, 5.23);

#if THREADY
	printf ("THREADY!!!!!\n");
	pthread_create (&tid, NULL, tsub, (void *)vobj);
	pthread_join (tid, NULL);
#else
	tsub (NULL);
#endif

	return 0;
}

