
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include <utmp.h>

FILE *file(char *name)
{
	FILE *ufp;

	if (!(ufp = fopen(name, "r"))) {
		err(1, "%s", name);
	}
	return(ufp);
}

int main(void)
{
#define NAME_WIDTH  8

	FILE *ufp;
	int numberOfUsers = 0;
	struct utmp usr;
	ufp = file(_PATH_UTMP);
	while (fread((char *)&usr, sizeof(usr), 1, ufp) == 1) {
		if (*usr.ut_name && *usr.ut_line && *usr.ut_line != '~') {
			if (strcmp (usr.ut_host, ":0") && strcmp (usr.ut_name, "LOGIN") && usr.ut_addr_v6[0]) {
				uint8_t *ipv4addr = (uint8_t *)&usr.ut_addr_v6;
				printf ("%s %s %s %hd %u.%u.%u.%u\n", usr.ut_name, usr.ut_host, usr.ut_line, usr.ut_type, ipv4addr[0], ipv4addr[1], ipv4addr[2], ipv4addr[3]);
				numberOfUsers++;
			}
		}
	}

	printf ("\nnUsers: %u\n", numberOfUsers);

	fclose (ufp);
	return 0;
}

