#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define DEBUG_PRINTF(module, format, args...) printf (format, ##args) 

#define PF(n, fmt, args...) \
	do { \
		if (n) { \
			if (*fmt) printf (fmt, ##args); \
			ret = errno; \
		} \
	} while (0)

#define FILES "conf" 

int main ()
{
	if (strlen (macrom)) printf ("yay!\n");
	else printf ("day!\n");

	return 0;
}
