
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char **argv)
{
	int ret = 0;
	char buf[8] = "";
	char *ptr = buf;
	fd_set readfds;
	int cnt;
	struct timeval tv = {1, 0};

	FD_ZERO (&readfds);
	FD_SET (0, &readfds);

	while (1) {
		ret = select (1, &readfds, NULL, NULL, &tv);
		if (ret < 0) {
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			printf ("tout!\n");
			FD_SET (0, &readfds);
		} else if (ret == 0) {
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			printf ("allowable tout!\n");
			FD_SET (0, &readfds);
		} else if (FD_ISSET (0, &readfds)) {
			int n = read (0, ptr, 1);
			if (*ptr != '\n') {
				printf ("read %d bytes: %c\n", n, *ptr);
				ptr += 1;
				if (cnt++ == 5)
					break;
			}
			FD_SET (0, &readfds);
		} else {
			printf (":");
			/*FD_SET (0, &readfds);*/
		}
	}
	printf ("\nfinal buf: %s\n", buf);
	return 0;
}
