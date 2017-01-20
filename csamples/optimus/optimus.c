
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "optimus.h"

const char *usage_str = "Usage: ./optimus <test> [additional-args]\n" \
						 "\tOptions: --circinc\n" \
						 "--\n";

static inline uint32_t circinc_cmp (uint32_t var, uint32_t lmt)
{
	return ((var + 1) == lmt) ? 0 : var + 1;
}

static inline uint32_t circinc_bits (uint32_t var, uint32_t lmt_mask)
{
	return (var + 1) & (lmt_mask);
}

static inline uint32_t circinc_if (uint32_t var, uint32_t lmt)
{
	var++;
	if (var == lmt) var = 0;
	return var;
}

static inline uint32_t circinc_mod (uint32_t var, uint32_t lmt)
{
	return ++var % lmt;
}

static inline int test_circinc (char **argv)
{
	typedef uint32_t (*circinc_t) (uint32_t, uint32_t);
	struct circinc_mode {
		char type[20];
		circinc_t func;
	};
	struct circinc_mode circinc_modes[] = {
		{"circinc_ift", circinc_if},
		{"circinc_cmp", circinc_cmp},
		{"circinc_bit", circinc_bits},
		{"circinc_mod", circinc_mod},
	};
    uint32_t i = 0, j, k = 10, fidx = 0;
    uint32_t lmt = 10;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent = 0;
	circinc_t func;

	for (fidx = 0; \
			fidx < sizeof (circinc_modes) / sizeof (*circinc_modes); \
			fidx++) {
		func = circinc_modes[fidx].func;
		type = circinc_modes[fidx].type;
		i = 0;
		for (k = 0; k < 10; k++) {
			start = clock ();
			for (j = 0; j < 10000000; j++)
				i = func (i, lmt);
			end = clock ();
			time_spent[k] = (end - start);
		}
		for (k = 0; k < 10; k++)
			total_time_spent += time_spent[k];
		printf ("clocks taken for test_circinc with %d[%s]: %f\n", \
				fidx, type, total_time_spent/10);
	}

	return 0;
}

int main (int argc, char **argv)
{
	int ret = 0;

	if (argc < 2) {
		ret = -1;
		goto BADLAND;
	}

	if (!strncmp ("--circinc", argv[1])) {
		if (test_circinc (argv)) goto BADLAND;
	/*} else if (!strncmp (argv[1], "")) {*/
	} else {
		ret = -2;
	}
BADLAND:
	if (ret) printf ("%s", usage_str);
	return ret;
}

