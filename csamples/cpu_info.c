#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	/*POSIX.1*/
	printf ("_SC_ARG_MAX: %ld\r\n", sysconf (_SC_ARG_MAX));
	printf ("_SC_CHILD_MAX: %ld\r\n", sysconf (_SC_CHILD_MAX));
	printf ("_SC_HOST_NAME_MAX: %ld\r\n", sysconf (_SC_HOST_NAME_MAX));
	printf ("_SC_LOGIN_NAME_MAX: %ld\r\n", sysconf (_SC_LOGIN_NAME_MAX));
	printf ("_SC_CLK_TCK: %ld\r\n", sysconf (_SC_CLK_TCK));
	printf ("_SC_OPEN_MAX: %ld\r\n", sysconf (_SC_OPEN_MAX));
	printf ("_SC_PAGESIZE: %ld\r\n", sysconf (_SC_PAGESIZE));
	printf ("_SC_RE_DUP_MAX: %ld\r\n", sysconf (_SC_RE_DUP_MAX));
	printf ("_SC_STREAM_MAX: %ld\r\n", sysconf (_SC_STREAM_MAX));
	printf ("_SC_SYMLOOP_MAX: %ld\r\n", sysconf (_SC_SYMLOOP_MAX));
	printf ("_SC_TTY_NAME_MAX: %ld\r\n", sysconf (_SC_TTY_NAME_MAX));
	printf ("_SC_TZNAME_MAX: %ld\r\n", sysconf (_SC_TZNAME_MAX));
	printf ("_SC_VERSION: %ld\r\n", sysconf (_SC_VERSION));
	/*POSIX.2*/
	printf ("_SC_BC_BASE_MAX: %ld\r\n", sysconf (_SC_BC_BASE_MAX));
	printf ("_SC_BC_DIM_MAX: %ld\r\n", sysconf (_SC_BC_DIM_MAX));
	printf ("_SC_BC_SCALE_MAX: %ld\r\n", sysconf (_SC_BC_SCALE_MAX));
	printf ("_SC_BC_STRING_MAX: %ld\r\n", sysconf (_SC_BC_STRING_MAX));
	printf ("_SC_COLL_WEIGHTS_MAX: %ld\r\n", sysconf (_SC_COLL_WEIGHTS_MAX));
	printf ("_SC_EXPR_NEST_MAX: %ld\r\n", sysconf (_SC_EXPR_NEST_MAX));
	printf ("_SC_LINE_MAX: %ld\r\n", sysconf (_SC_LINE_MAX));
	printf ("_SC_RE_DUP_MAX: %ld\r\n", sysconf (_SC_RE_DUP_MAX));
	printf ("_SC_2_VERSION: %ld\r\n", sysconf (_SC_2_VERSION));
	printf ("_SC_2_C_DEV: %ld\r\n", sysconf (_SC_2_C_DEV));
	printf ("_SC_2_FORT_DEV: %ld\r\n", sysconf (_SC_2_FORT_DEV));
	printf ("_SC_2_FORT_RUN: %ld\r\n", sysconf (_SC_2_FORT_RUN));
	printf ("_SC_2_LOCALEDEF: %ld\r\n", sysconf (_SC_2_LOCALEDEF));
	printf ("_SC_2_SW_DEV: %ld\r\n", sysconf (_SC_2_SW_DEV));
	/*Following May or May not be standard*/
	printf ("_SC_PHYS_PAGES: %ld\r\n", sysconf (_SC_PHYS_PAGES));
	printf ("_SC_AVPHYS_PAGES: %ld\r\n", sysconf (_SC_AVPHYS_PAGES));
	printf ("_SC_NPROCESSORS_CONF: %ld\r\n", sysconf (_SC_NPROCESSORS_CONF));
	printf ("_SC_NPROCESSORS_ONLN: %ld\r\n", sysconf (_SC_NPROCESSORS_ONLN));

	return 0;
}
