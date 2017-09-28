
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include <utmp.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <execinfo.h>
#include <signal.h>

#define get_reg(name) \
	({ register uint64_t regs asm (#name); regs; })

uint64_t getsp( void )
{
	uint64_t sp;
	asm( "mov %%rsp, %0" : "=rm" ( sp ));
	return sp;
}

void print_regs (int sig)
{
	printf ("SEGFAULT!!!!\n");
#if 0
	printf ("sp: %lx\n", getsp ());
#else
	printf ("sp: %lx\n", get_reg (sp));
#endif
	exit (-1);
}

int main(int argc, char *argv[])
{
	int count = 100;
	int n;
	signal (SIGSEGV, print_regs);

	n = *((int *)NULL);
	while (count--) {
		printf ("sleeping in main at iter: %d\n", count);
		sleep (1);
	}
	return 0;
}

