#include <stdint.h>
#include <stdio.h>

int main()
{
	uint8_t mac[6] = {1};

	if (!(mac[0]^mac[1]^mac[2]^mac[3]^mac[4]^mac[5]))
		printf ("no mac!\n");
	else
		printf ("hi mac!\n");
}
