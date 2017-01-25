
#include "optimus.h"

static inline int sigunsig_signed (int x, int y)
{
	return (x < y);
}

static inline int sigunsig_unsign (int x, int y)
{
	return ((unsigned)x < (unsigned)y);
}

int test_sigunsig (uint8_t argc, char **argv)
{
	typedef int (*sigunsig_func_t)(int, int);
	struct sigunsig_mode {
		char type[20];
		sigunsig_func_t func;
	};
	struct sigunsig_mode sigunsig_modes[] = {
		{"sigunsig_signed", sigunsig_signed},
		{"sigunsig_unsign", sigunsig_unsign},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	sigunsig_func_t func;
	int x = 3, y = 8;
	int in_range;

	for (fidx = 0; fidx < SIZEOF_ARRAY (sigunsig_modes); fidx++) {
		func = sigunsig_modes[fidx].func;
		type = sigunsig_modes[fidx].type;
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 10000000; i++)
				in_range = func (x, y);
			end = clock ();
			time_spent[j] = (end - start);
		}
		total_time_spent = 0;
		for (j = 0; j < 10; j++)
			total_time_spent += time_spent[j];
		printf ("clocks taken for [%s]: %f.. and it is%sin range\n", \
				type, total_time_spent/10, in_range ? " " : " not ");
	}

	return 0;
}
