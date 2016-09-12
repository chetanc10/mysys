#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
/*#include <ioctl.h>*/

#define HELLO_MAGIC 'k'

#define HELLO_IOC_KMALLOC _IO(HELLO_MAGIC, 0)

int main ()
{
	int fd, n;

	fd = open ("/dev/hello", O_RDWR);
	if (fd < 0) {
		perror ("/dev/hello");
		return -1;
	}

#if 0
	if (4 != read (fd, &n, 4)) {
		perror ("/dev/hello");
		close (fd);
		return -2;
	}

	if (4 != write (fd, &n, 4)) {
		perror ("/dev/hello");
		close (fd);
		return -3;
	}
#endif

	if (ioctl (fd, HELLO_IOC_KMALLOC, &fd) < 0) {
		perror ("/dev/hello");
		close (fd);
		return -4;
	}

	close (fd);
	return 0;
}

