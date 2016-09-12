
#define _GNU_SOURCE

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
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

char buf[256];

#if 1
int logf_write (const char *fmt, ...)
{
    int ret;
    va_list myargs;

    va_start(myargs, fmt);

    ret = vsprintf (buf, fmt, myargs);

    printf ("buf: %s\n", buf);

    va_end(myargs);

    return ret;
}
#else
#define logf_write(fmt, ...) \
{ \
    /*sprintf (buf, "@%d-%s:%s"fmt"\n", __LINE__, __func__, __FILE__, ##__VA_ARGS__);*/ \
    sprintf (buf, fmt"\n", ##__VA_ARGS__); \
    printf ("buf: %s\n", buf); \
}
#endif

int main(int argc, char *argv[])
{
    int n = 345;
    while (1) {
	sleep (1);
	printf ("n\n");
	logf_write ("slept a while: %d", n++);
    }
    return 0;
}

