
#include "optimus.h"

static inline uint32_t minmax_ltgt (uint32_t x, uint32_t min, uint32_t max)
{
	return ((x >= min) && (x <= max));
}

static inline uint32_t minmax_diff (uint32_t x, uint32_t min, uint32_t max)
{
	return ((x - min) < (max - min));
}

int test_minmax (uint8_t argc, char **argv)
{
	typedef uint32_t (*minmax_func_t)(uint32_t, uint32_t, uint32_t);
	struct minmax_mode {
		char type[20];
		minmax_func_t func;
	};
	struct minmax_mode minmax_modes[] = {
		{"minmax_ltgt", minmax_ltgt},
		{"minmax_diff", minmax_diff},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent = 0;
	minmax_func_t func;
	uint32_t x, min, max;
	uint32_t in_range;

	if (argc != 3) {
		return -3;
	}

	x = atoi (argv[0]);
	min = atoi (argv[1]);
	max = atoi (argv[2]);

	for (fidx = 0; fidx < SIZEOF_ARRAY (minmax_modes); fidx++) {
		func = minmax_modes[fidx].func;
		type = minmax_modes[fidx].type;
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 10000000; i++)
				in_range = func (x, min, max);
			end = clock ();
			time_spent[j] = (end - start);
		}
		for (j = 0; j < 10; j++)
			total_time_spent += time_spent[j];
		printf ("clocks taken for [%s]: %f.. and it is%sin range\n", \
				type, total_time_spent/10, in_range ? " " : " not ");
	}

	return 0;
}
