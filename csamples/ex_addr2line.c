#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BT_BUF_SIZE 100

char *pname;

int addr2line(char const * const program_name, void const * const addr)
{
	char addr2line_cmd[512] = {0};

	/* have addr2line map the address to the relent line in the code */
#ifdef __APPLE__
	/* apple does things differently... */
	sprintf(addr2line_cmd,"atos -o %.256s %p", program_name, addr); 
#else
	sprintf(addr2line_cmd,"addr2line -f -p -e %.256s %p", program_name, addr); 
#endif

	/* This will print a nicely formatted string specifying the
	   function and source line of the address */
	return system (addr2line_cmd);
}

void myfunc3(void)
{
	int j, nptrs;
	void *buffer[BT_BUF_SIZE];
	void **strings;

	nptrs = backtrace(buffer, BT_BUF_SIZE);
	printf("backtrace() returned %d addresses\n", nptrs);

	/* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
	   would produce similar output to the following: */

	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL) {
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}

	for (j = 0; j < nptrs - 2; j++) {
#if 0
		printf("%s\n", strings[j]);
		if (0)
		{
			char ch;
			printf ("next? <c>: ");
			scanf (" %c", &ch);
		}
#else
#if 0
		printf("%s\n", strings[j]);
		{
			char hstr[512];
			sprintf (hstr, "addr2line -f -p -e %s %p", "a.out", buffer[j]);
			/*printf ("do: %s\n", hstr);*/
			system (hstr);
		}
#endif
		if (addr2line (pname, buffer[j])) {
			printf ("couldn't get human readable lines from backtrace\n");
		}
#endif
	}

	free(strings);
}

static void myfunc2(void)
{
	myfunc3();
}

void myfunc(int ncalls)
{
	if (ncalls > 1)
		myfunc(ncalls - 1);
	else
		myfunc2();
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "%s num-calls\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	pname = argv[0];
	myfunc(atoi(argv[1]));
	exit(EXIT_SUCCESS);
}
