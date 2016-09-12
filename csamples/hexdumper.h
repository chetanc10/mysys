
#ifndef __COMMON_H
#define __COMMON_H

#include <stdlib.h>
#include <stdint.h>

#ifndef MIN
#define MIN(a,b) (a<b)?a:b
#endif


#if 1
#define hexDump_nBit(nb, buf, sz) \
	do { \
		uint16_t n##nb = 0; \
		uint##nb##_t *lptr = (uint##nb##_t *)buf; \
		for (; n##nb < sz; n##nb++) { \
			printf ("%#020x%s", *lptr++, (n##nb+1)%1?" ":"\r\n"); \
		} \
	} while (0)
// 			printf ("%08x%s", *buf++, (n32+1)%1?" ":"\r\n");
#define hexDump_32Bit(buf, sz) hexDump_nBit(32, buf, sz)
#define hexDump_16Bit(buf, sz) hexDump_nBit(16, buf, sz)
#else
static inline void hexDump_32Bit (uint32_t *buf, uint16_t size)
{
	uint16_t n32 = 0;
	for (; n32 < size; n32++) {
		printf ("%08x%s", *buf++, (n32+1)%1?" ":"\r\n");
	}
}
#endif

void *g_malloc(size_t n_bytes)
{    
	void *mem;
	//     __coverity_negative_sink__(n_bytes);
	mem = malloc(n_bytes == 0 ? 1 : n_bytes);
	//     if (!mem) __coverity_panic__();
	return mem;
}                               

void *g_malloc0(size_t n_bytes)
{
	void *mem;
	//     __coverity_negative_sink__(n_bytes);
	mem = calloc(1, n_bytes == 0 ? 1 : n_bytes);
	//     if (!mem) __coverity_panic__();
	return mem;
}

void g_free(void *mem)
{
	free(mem);
}

#endif /*#ifndef __COMMON_H*/

