
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "optimus.h"

const char *usage_str = "Usage: ./optimus <test> <type> [additional-args]\n" \
						 "\tOptions: --circinc <--cmp | --bits | --if>\n" \
						 "--\n";

static inline void circinc_cmp (uint32_t var, uint32_t lmt)
{
	var = ((var + 1) == lmt) ? 0 : var + 1;
}

static inline void circinc_bits (uint32_t var, uint32_t lmt_mask)
{
	var = (var + 1) & (lmt_mask);
}

static inline void circinc_if (uint32_t var, uint32_t lmt)
{
	var++;
	if (var == lmt) var = 0;
}

static inline int test_circinc (char **argv)
{
    uint32_t i, j, k = 10;
    uint32_t lmt = 10;
	clock_t start, end;
	double time_spent[10], total_time_spent = 0;
	void (*func) (uint32_t, uint32_t);

	if (!strncmp ("--cmp", argv[2])) {
		func = circinc_cmp;
	} else if (!strncmp ("--bits", argv[2])) {
		func = circinc_bits;
		lmt--;
	} else if (!strncmp ("--if", argv[2])) {
		func = circinc_if;
	} else {
		return -3;
	}

	while (k--) {
		start = clock ();
		for (j = 0; j < 100000000; j++)
			func (i, lmt);
		end = clock ();
		time_spent[k] = (end - start);
	}

	for (k = 0; k < 10; k++)
		total_time_spent += time_spent[k];

	printf ("clocks taken for test_circinc with %s: %f\n", argv[2], total_time_spent/10);

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

