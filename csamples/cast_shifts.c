
#include <stdio.h>

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <pthread.h>
#include <semaphore.h>
#include <limits.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/syscall.h>
#include <math.h>

typedef struct {
	uint8_t *a;
	uint8_t *b;
} a_t;


int main (int argc, char **argv)
{
	uint32_t lsb, msb;
	a_t a = {(uint8_t *)&lsb, (uint8_t *)&msb};

	/*printf ("%x, %x\n", lsb, msb);*/
	lsb = (uint32_t)((uint64_t)(a.a) & 0xffffffff);
	msb = (uint32_t)((uint64_t)(a.a) >> 32);

	printf ("%x, %x\n", lsb, msb);
	return 0;
}

