
#include "optimus.h"

static inline uint32_t remainder_bit (uint32_t x, uint32_t lmt_mask)
{
	return x & lmt_mask;
}

static inline uint32_t remainder_mod (uint32_t x, uint32_t lmt)
{
	return ++x % lmt;
}

int test_remainder (uint8_t argc, char **argv)
{
	typedef uint32_t (*remainder_func_t) (uint32_t, uint32_t);
	struct remainder_mode {
		char type[20];
		remainder_func_t func;
	};
	struct remainder_mode remainder_modes[] = {
		{"remainder_bit", remainder_bit},
		{"remainder_mod", remainder_mod},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	remainder_func_t func;
	uint32_t x, lmt;

	if (argc != 2) {
		return -3;
	}

	lmt = atoi (argv[1]);

	for (fidx = 0; fidx < SIZEOF_ARRAY (remainder_modes); fidx++) {
		func = remainder_modes[fidx].func;
		type = remainder_modes[fidx].type;
		x = atoi (argv[0]);
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 10000000; i++)
				x = func (x, lmt);
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

