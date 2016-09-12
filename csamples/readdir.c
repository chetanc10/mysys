
#include <stdio.h>

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
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

int main ()
{
	DIR *dir;
	struct dirent *dp;
	char * file_name;

	dir = opendir (".");

	while ((dp=readdir (dir)) != NULL) {
		if (!strcmp (dp->d_name, ".") \
				|| !strcmp (dp->d_name, "..")) {
			// do nothing (straight logic)
		} else {
			file_name = dp->d_name; // use it
			printf ("%s\t",file_name);
		}
	}
	printf ("\n");

	closedir (dir);

	return 0;
}
