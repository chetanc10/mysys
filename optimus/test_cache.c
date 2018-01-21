
#include "optimus.h"

#define DESC_SZ 128
#define PIGS_SZ 128

typedef struct {
	char desc[DESC_SZ];
	uint8_t mac[6];
	uint8_t ip[4];
	uint32_t dtim_interval;
} biggy_t;

biggy_t pigs[PIGS_SZ] = {0};

static inline void rte_prefetch0(const volatile void *p)
{   
    asm volatile ("prefetcht0 %[p]" : : [p] "m" (*(const volatile char *)p));
}

static inline void cache_nofetch (void)
{
	int i = 0;

	for (i = 0; i < PIGS_SZ; i++) {
		pigs[i].dtim_interval = i;
		pigs[i].ip[0] = 0;
		pigs[i].ip[1] = 1;
		pigs[i].ip[2] = 2;
		pigs[i].ip[3] = 3;
		_mm_clflush (pigs[i].ip);
	}
}

static inline void cache_prefetch (void)
{
	int i = 0;

	for (i = 0; i < PIGS_SZ; i++) {
		pigs[i].dtim_interval = i;
		rte_prefetch0 (pigs[i].ip);
		pigs[i].ip[0] = 0;
		pigs[i].ip[1] = 1;
		pigs[i].ip[2] = 2;
		pigs[i].ip[3] = 3;
		_mm_clflush (pigs[i].ip);
	}
}

int test_cache (uint8_t argc, char **argv)
{
	typedef void (*cache_func_t) ();
	struct cache_mode {
		char type[20];
		cache_func_t func;
	};
	struct cache_mode cache_modes[] = {
#if 1
		{"cache_nofetch", cache_nofetch},
#else
		{"cache_prefetch", cache_prefetch},
#endif
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	cache_func_t func;

	for (fidx = 0; fidx < SIZEOF_ARRAY (cache_modes); fidx++) {
		func = cache_modes[fidx].func;
		type = cache_modes[fidx].type;
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 1000000; i++)
				func ();
			end = clock ();
			time_spent[j] = (end - start);
		}
		total_time_spent = 0;
		for (j = 0; j < 10; j++)
			total_time_spent += time_spent[j];
		printf ("clocks taken for [%s]: %f\n", type, total_time_spent/10);
	}

	return 0;
}

