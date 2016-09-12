#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

static inline int clz32(uint32_t val)
{
#if 0//QEMU_GNUC_PREREQ(3, 4)
	return val ? __builtin_clz(val) : 32;
#else
	/* Binary search for the leading one bit.  */
	int cnt = 0;

	if (!(val & 0xFFFF0000U)) {
		cnt += 16;
		val <<= 16;
	}
	if (!(val & 0xFF000000U)) {
		cnt += 8;
		val <<= 8;
	}
	if (!(val & 0xF0000000U)) {
		cnt += 4;
		val <<= 4;
	}
	if (!(val & 0xC0000000U)) {
		cnt += 2;
		val <<= 2;
	}
	if (!(val & 0x80000000U)) {
		cnt++;
		val <<= 1;
	}
	if (!(val & 0x80000000U)) {
		cnt++;
	}
	return cnt;
#endif
}

int main(uint8_t argc, char **argv)
{
	if (argc == 2)
		printf ("%d\r\n", clz32(atoi(argv[1])));

	printf ("%u\r\n", ~(524288-1));
}
