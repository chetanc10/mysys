
/**
 * Run: ./a.out <any alphanumeric input without SPACE>
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

int main (int argc, char **argv)
{
	int len;
	const char acceptables[] = "ABCDEFGHIJKLMNOPQRSTWXYZabcdefghijklmnopqrstwxyz0123456789(*&^%$#@!_+)";
	char *str;

	if (argc != 2) {
		printf ("Usage: ./a.out <any alphanumeric input without SPACE>\n");
		return -1;
	}

	str = argv[1];
	len = strspn(str, acceptables);

	/*printf("First matched character is at %d, len: %lu\n", len, strlen (str));*/

	if (len != strlen (str)) {
		printf ("Damn!\n");
		return -1;
	}

	printf ("Whoa!\n");
	return 0;
}

