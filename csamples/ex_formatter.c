
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf ("Usage: ./a.out <format - x|c|i|u> <input-num-or-char>");
		return -1;
	}

	char f1 = argv[1][0];
	int32_t i32;
	uint32_t u32, hex;
	char ch;

	switch (f1) {
		case 'x': hex = strtoul (argv[2], NULL, 16);
				  printf("int: %d\r\nuint: %u\r\nhex: 0x%x\r\nchar: %c\r\n", (int32_t)hex, (uint32_t)hex, hex, hex); break;
		case 'c': ch = argv[2][0];
				  printf("int: %d\r\nuint: %u\r\nch: 0x%x\r\nchar: %c\r\n", (int32_t)ch, (uint32_t)ch, ch, ch); break;
				  // 		case 'i': i32 = (int32_t) strtoul (argv[2], NULL, 10);
		case 'i': i32 = (int32_t) atoi(argv[2]);
				  printf ("typecasted: %u\r\n", ((uint32_t)i32)%128);
				  printf("int: %d\r\nuint: %u\r\ni32: 0x%x\r\nchar: %c\r\n", (int32_t)i32, (uint32_t)i32, i32, i32); break;
		case 'u': u32 = (uint32_t) strtoul (argv[2], NULL, 10); break;
				  printf("int: %d\r\nuint: %u\r\nu32: 0x%x\r\nchar: %c\r\n", (int32_t)u32, (uint32_t)u32, u32, u32); break;
		default: break;
	}

}
