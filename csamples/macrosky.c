#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>

#define wmb()   asm volatile("sfence" ::: "memory")

#define IMPLIES(x, y) (!(x) || (y))

#define LSB(x) ((x) ^ ((x) - 1) & (x))

#define me1 01
#define me2 02
#define me3 03

#define me me1

int main()
{
	int n;
	printf ("%d\n", n = IMPLIES (6, 6));
	printf ("%d\n", LSB(6));
	return 0;
}

