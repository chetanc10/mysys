
#include "optimus.h"

static inline int condret_atlast (int a, int b)
{
	if (a < b) {
		return 0;
	}

	a -= b;

	return 0;
}

static inline int condret_onspot (int a, int b)
{
	if (a >= b) {
		a -= b;
	}

	return 0;
}

int test_condret (uint8_t argc, char **argv)
{
	typedef int (*condret_func_t) (int, int);
	struct condret_mode {
		char type[20];
		condret_func_t func;
	};
	struct condret_mode condret_modes[] = {
		{"condret_atlast", condret_atlast},
		{"condret_onspot", condret_onspot},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	condret_func_t func;
	int a, b;

	if (argc != 2) {
		return -3;
	}

	a = atoi (argv[0]);
	b = atoi (argv[1]);

	for (fidx = 0; fidx < SIZEOF_ARRAY (condret_modes); fidx++) {
		func = condret_modes[fidx].func;
		type = condret_modes[fidx].type;
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 100000000; i++)
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

