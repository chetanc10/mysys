
#include "optimus.h"

static inline int absval_opcmp (int a, int b)
{
	return (a < b) ? (b - a) : (a - b);
}

static inline int absval_rescmp (int a, int b)
{
	return abs(a - b);
}

int test_absval (uint8_t argc, char **argv)
{
	typedef int (*absval_func_t) (int, int);
	struct absval_mode {
		char type[20];
		absval_func_t func;
	};
	struct absval_mode absval_modes[] = {
		{"absval_opcmp", absval_opcmp},
		{"absval_rescmp", absval_rescmp},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	absval_func_t func;
	int a, b;

	if (argc != 2) {
		return -3;
	}

	a = atoi (argv[0]);
	b = atoi (argv[1]);

	for (fidx = 0; fidx < SIZEOF_ARRAY (absval_modes); fidx++) {
		func = absval_modes[fidx].func;
		type = absval_modes[fidx].type;
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 10000000; i++)
				func (a, b);
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

