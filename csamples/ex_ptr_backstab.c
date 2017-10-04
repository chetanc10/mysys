
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

typedef struct {
	uint32_t n1;
	uint32_t n2;
	const uint32_t * const intptr;
} s_t;

s_t *s;

void corrupt (s_t *s)
{
	memset ((uint8_t *)s + 1, 0, sizeof (*s) - 1);
}

void backstab (uint32_t **ptr, uint64_t addr)
{
	/*memcpy (*ptr, addr, sizeof (ptr));*/
	*ptr = (uint32_t *)addr;
}

int main(int argc, char *argv[])
{
	s = calloc (1, sizeof (*s));
	uint32_t *tmpintptr = (uint32_t *)((uint8_t *)s + offsetof (s_t, intptr));
	/*s->intptr = &s->n1;*/
	printf ("n1: %p, n2: %p, intptr: %p\n", &s->n1, &s->n2, tmpintptr);
	backstab ((uint32_t **)&s->intptr, (uint64_t)&s->n1);
	/*backstab ((uint32_t **)tmpintptr, (uint64_t)&s->n1);*/
	s->n1 = 0x5a5a5a5a;
	s->n2 = 0xa5a5a5a5;
	printf ("%x, %p, %x\n", s->n1, s->intptr, s->n2);
	/*corrupt (s);*/
	/**(s->intptr) = 123;*/
	/*printf ("%x, %p, %x\n", s->n1, s->intptr, s->n2);*/
	free (s);
	return 0;
}

