
#define _GNU_SOURCE

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sched.h>

int main (int argc, char **argv)
{
	int ret;
	cpu_set_t cpuset;
	int cpumask = 0;
	int i = 0;
	pid_t pid;

	if (argc != 3) {
		printf ("Usage: ./a.out <pid> < - | {cpuset: 1 to f hex} > \n");
		return -1;
	}

	pid = (pid_t) strtoul (argv[1], NULL, 10);

	if (argv[2][0] == '-') {
		ret = sched_getaffinity (pid, sizeof (cpuset), &cpuset);
		while (i < 0x0f) {
			if (CPU_ISSET (i, &cpuset))
				cpumask |= (1 << i);
			i++;
		}
		printf ("cpuset: %x\n", cpumask);
		return ret? 0 : cpumask;
	} else {
		cpumask = (int)strtoul (argv[2], NULL, 16);
		if (cpumask > 0x0f) {
			printf ("Be in ur limits!\n");
			ret = -1;
		} else {
			CPU_ZERO (&cpuset);
			while (i < 0x0f) {
				if (cpumask & (1 << i) & 0x0f)
					CPU_SET (i, &cpuset);
				i++;
			}
			ret = sched_setaffinity (pid, sizeof (cpuset), &cpuset);
			if (ret) perror ("set-affinity");
		}
		return ret? 0 : cpumask;
	}
}
