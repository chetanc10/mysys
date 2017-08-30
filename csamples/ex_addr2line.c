
/**
 * Compilation : gcc ex_addr2line
 * Run         : ./a.out <number-of-func-calls>
 * Status      : Yet to clarify some things
 */

#include <execinfo.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUITLTINS 0

#define BT_BUF_SIZE 100

#define USING_BASH_SYSTEM_FUNC 0

char *progname;

void my_addr2line(char *progname, void *addr)
{
    char addr2line_cmd[512] = {0};

    sprintf(addr2line_cmd,"addr2line -f -p -e %.256s %p", progname, addr); 
#if USING_BASH_SYSTEM_FUNC
    system (addr2line_cmd);
#else
    {
	FILE *fp;
	fp = popen(addr2line_cmd, "r");
	if (fp) {
	    char buf[128];
	    while (fgets(buf, sizeof(buf), fp)) {
		if (!strstr (buf, "??"))
		    printf("%s", buf); 
	    }
	} else {
	    perror (addr2line_cmd);
	}
    }
#endif
}

void strackers(void)
{
#if BUITLTINS
    /* Might not be portable/flexible, but it's another way to do it! */
    my_addr2line(__builtin_return_address(0));
    my_addr2line(__builtin_return_address(1));
    my_addr2line(__builtin_return_address(2));
    my_addr2line(__builtin_return_address(3));
#else
    int j, nptrs;
    void *buffer[BT_BUF_SIZE];
    void **strings;

    nptrs = backtrace(buffer, BT_BUF_SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
       would produce similar output to the following: */

    strings = (void **)backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
	perror("backtrace_symbols");
	exit(EXIT_FAILURE);
    }

    for (j = 0; j < nptrs - 2; j++) {
	/*printf("%s\n", (char *)strings[j]);*/
	my_addr2line (progname, buffer[j]);
    }

    free(strings);
#endif
}

static void myfunc2(void)
{
    strackers();
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

    progname = argv[0];
    myfunc(atoi(argv[1]));
    exit(EXIT_SUCCESS);
}
