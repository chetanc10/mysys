#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/times.h>

#if defined(__linux__) && !defined(__KERNEL__)
#define yy 7
#endif

typedef struct regbits {
	uint16_t bits1:4;
	uint16_t bits2:4;
	uint16_t bits3:4;
	uint16_t bits4:4;
} regbits;

typedef struct regvals {
	uint16_t vals;
} regvals;

int main (int argc, char **argv)
{
	unsigned int i, j;
	unsigned long long k=999999999999999999;
	unsigned long long l=999999999;

#if 1
	regbits bb = {3, 7, 6};
	for(;l;l--)
		for(i=0;i<k;i++){
			for(j=0;j<k;j++){
				if (bb.bits3 != 5) {
				} else
					bb.bits3 = 6;
			}
		}
	printf ("bits!!\r\n");
#else
	regvals vv = {3 | 7 << 4 | 6 << 8};
	for(;l;l--)
		for(i=0;i<k;i++){
			for(j=0;j<k;j++){
				if ((vv.vals >> 8) && 0xf != 5) {
				} else
					vv.vals |= (6 << 8);
			}
		}
	printf ("vals!!\r\n");
#endif
	return 0;        
}
