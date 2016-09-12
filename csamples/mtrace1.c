#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <mcheck.h>

int main(int argc, char *argv[])
{
	char *p1 = NULL, *p2 = NULL, *p3 = NULL;
	char ch = 1;

	mtrace();

	while (ch) {
		printf ("\n> ");
		scanf (" %c", &ch);
		switch (ch) {
			case '1':
				if (!p1) p1 = calloc (1, 20);
				else {free(p1); p1 = NULL;}
				break;
			case '2':
				if (!p2) p2 = calloc (1, 20);
				else {free(p2); p2 = NULL;}
				break;
			case '3':
				if (!p3) p3 = calloc (1, 20);
				else {free(p3); p3 = NULL;}
				break;
			case '0':
				ch = 0;
				break;
			default:
				break;
		}
	}

	exit(EXIT_SUCCESS);
}
