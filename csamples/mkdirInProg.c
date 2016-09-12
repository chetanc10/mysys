#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	if(!argv[1]) {
		printf("poraa!\n");
		return 0;
	}
	char *dir = "CtaN";
	char str[20] = "";
	strcpy(str, "mkdir ");
	strcat(str, argv[1]);
	system(str);
}
