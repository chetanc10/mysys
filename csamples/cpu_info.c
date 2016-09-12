#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char **argv)
{
	printf ("nCPUsConf: %ld\r\n", sysconf (_SC_NPROCESSORS_CONF));
	printf ("nCPUsONLN: %ld\r\n", sysconf (_SC_NPROCESSORS_ONLN));
	printf ("nPHYpages: %ld\r\n", sysconf (_SC_PHYS_PAGES));
	printf ("nPageSize: %ld\r\n", sysconf (_SC_PAGESIZE));

	printf ("%u::\r\n", sizeof (unsigned long));
	printf ("%llx\r\n", (uint64_t)~0);
	return 0;
}
