
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

#define USING_HDUMP_FMACRO 1

#if USING_HDUMP_FMACRO

#define EN_CDUMP 1
#define DIS_CDUMP 0

#define _dumpf64(elem) printf ("%016lx ", elem)
#define _dumpf32(elem) printf ("%08x ", elem)
#define _dumpf16(elem) printf ("%04x ", elem)
#define _dumpf8(elem)  printf ("%02x ", elem)

#define _cdump(buf) \
{ \
	uint32_t i; \
	printf ("  |"); \
	for (i = 0; i < 16; i++) { \
		printf ("%c", isprint ((buf)[i]) && ((buf)[i] < 0x80) ? (buf)[i] : '.'); \
	} \
	printf ("|\r\n"); \
}

#define hexDump_nBit(nb, buf, netries, do_cdump) \
	do { \
		uint##nb##_t nbe = netries/(nb>>3) +  netries%(nb>>3);\
		uint32_t i = 0, lmt = 128/nb; \
		uint##nb##_t *lptr = (uint##nb##_t *)buf; \
		char *cptr = (char *)lptr; \
		for (; i < nbe; i++) { \
			_dumpf##nb (lptr[i]); \
			if (!((i+1) % lmt)) { \
				if (do_cdump) { \
					/*printf ("@%u ", i);*/ \
					_cdump (cptr); \
					cptr += 16; \
				} else { \
					printf ("\r\n"); \
				} \
			} \
		} \
	} while (0)

#define hexDump_64Bit(buf, nentries, do_cdump) hexDump_nBit(64, buf, nentries, do_cdump)
#define hexDump_32Bit(buf, nentries, do_cdump) hexDump_nBit(32, buf, nentries, do_cdump)
#define hexDump_16Bit(buf, nentries, do_cdump) hexDump_nBit(16, buf, nentries, do_cdump)
#define hexDump_8Bit(buf, nentries, do_cdump) hexDump_nBit(8, buf, nentries, do_cdump)

#else

/*nentry correction needed -TODO*/
/*cdump for static hexdumper functions yet to be added -TODO*/
static inline void hexDump_64Bit (void *buf, uint32_t size, uint32_t do_cdump)
{
	uint64_t *buf64 = (uint64_t *)buf;
	uint32_t n64 = 0;
	for (; n64 < size; n64++) {
		printf ("%08lx%s", *buf64++, (n64+1)%2?" ":"\r\n");
	}
}
static inline void hexDump_32Bit (void *buf, uint32_t size, uint32_t do_cdump)
{
	uint32_t *buf32 = (uint32_t *)buf;
	uint32_t n32 = 0;
	for (; n32 < size; n32++) {
		printf ("%08x%s", *buf32++, (n32+1)%4?" ":"\r\n");
	}
}
static inline void hexDump_16Bit (void *buf, uint32_t size, uint32_t do_cdump)
{
	uint16_t *buf16 = (uint16_t *)buf;
	uint32_t n16 = 0;
	for (; n16 < size; n16++) {
		printf ("%04x%s", *buf16++, (n16+1)%8?" ":"\r\n");
	}
}
static inline void hexDump_8Bit (void *buf, uint32_t size, uint32_t do_cdump)
{
	uint8_t *buf8 = (uint8_t *)buf;
	uint32_t n8 = 0;
	for (; n8 < size; n8++) {
		printf ("%02x%s", *buf8++, (n8+1)%16?" ":"\r\n");
	}
}

#endif

int main (int argc, char **argv)
{
	int bits, cdump;
	char buf[256] = {'a', 'b', 'c', 'd', 'e', 'f', '0', 'a', 'b', 'c', 'd', 'e', 'f', '0', 'a', 'b'};

	if (argc != 3) {
		printf ("Usage: ./a.out <num-bits for integer hexdump - 8|16|32|64> <en|dis chardump - 1|0>\n");
		return -1;
	}

	bits = atoi (argv[1]);
	cdump = atoi (argv[2]);
	switch (bits) {
		case 8: hexDump_8Bit (buf, 32, cdump); break;
		case 16: hexDump_16Bit (buf, 32, cdump); break;
		case 32: hexDump_32Bit (buf, 32, cdump); break;
		case 64: hexDump_64Bit (buf, 32, cdump); break;
		default: printf ("Invalid: %d\n", bits); return -1;
	}

	return 0;
}

