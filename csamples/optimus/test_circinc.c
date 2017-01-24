
#include "optimus.h"

static inline uint32_t circinc_cmp (uint32_t x, uint32_t lmt)
{
	return ((x + 1) == lmt) ? 0 : x + 1;
}

static inline uint32_t circinc_bits (uint32_t x, uint32_t lmt_mask)
{
	return (x + 1) & (lmt_mask);
}

static inline uint32_t circinc_if (uint32_t x, uint32_t lmt)
{
	x++;
	if (x == lmt) x = 0;
	return x;
}

static inline uint32_t circinc_mod (uint32_t x, uint32_t lmt)
{
	return ++x % lmt;
}

int test_circinc (uint8_t argc, char **argv)
{
	typedef uint32_t (*circinc_func_t) (uint32_t, uint32_t);
	struct circinc_mode {
		char type[20];
		circinc_func_t func;
	};
	struct circinc_mode circinc_modes[] = {
		{"circinc_ift", circinc_if},
		{"circinc_cmp", circinc_cmp},
		{"circinc_bit", circinc_bits},
		{"circinc_mod", circinc_mod},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent = 0;
	circinc_func_t func;
	uint32_t x, lmt;

	if (argc != 2) {
		return -3;
	}

	lmt = atoi (argv[1]);

	for (fidx = 0; fidx < SIZEOF_ARRAY (circinc_modes); fidx++) {
		func = circinc_modes[fidx].func;
		type = circinc_modes[fidx].type;
		x = atoi (argv[0]);
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 10000000; i++)
				x = func (x, lmt);
			end = clock ();
			time_spent[j] = (end - start);
		}
		for (j = 0; j < 10; j++)
			total_time_spent += time_spent[j];
		printf ("clocks taken for [%s]: %f\n", type, total_time_spent/10);
	}

	return 0;
}

