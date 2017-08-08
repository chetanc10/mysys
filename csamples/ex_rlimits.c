
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>

int main (int argc, char **argv)
{
	struct rlimit rl = {0};

	if (getrlimit (RLIMIT_NOFILE, &rl) < 0) {
		perror ("getrlimit fsize: ");
		return -1;
	}
	printf ("cur:%lu max:%lu\n", rl.rlim_cur, rl.rlim_max);

	rl.rlim_cur = rl.rlim_max = 5;
	if (setrlimit (RLIMIT_NOFILE, &rl) < 0) {
		perror ("setrlimit fsize: ");
		return -2;
	}

	if (getrlimit (RLIMIT_NOFILE, &rl) < 0) {
		perror ("getrlimit fsize: ");
		return -1;
	}
	printf ("now, cur:%lu max:%lu\n", rl.rlim_cur, rl.rlim_max);

	int fd1 = open ("./file1", O_RDWR|O_CREAT);
	if (fd1 < 0) perror ("file1: ");
	int fd2 = open ("./file2", O_RDWR|O_CREAT);
	if (fd2 < 0) perror ("file2: ");
	int fd3 = open ("./file3", O_RDWR|O_CREAT);
	if (fd3 < 0) perror ("file3: ");
	int fd4 = open ("./file4", O_RDWR|O_CREAT);
	if (fd4 < 0) perror ("file4: ");

	close (fd1);
	close (fd2);
	close (fd3);
	close (fd4);

	return 0;
}
