
#include "optimus.h"

static inline int ifcond_signed (int x, int y, int lmt)
{
	if ((x + y) <= lmt)
		return 1;
	else
		return 0;
}

static inline int ifcond_unsign (int x, int y, int lmt)
{
	if ((unsigned)(x + y) <= (unsigned)lmt)
		return 1;
	else
		return 0;
}

int test_ifcond (uint8_t argc, char **argv)
{
	typedef int (*ifcond_func_t)(int, int, int);
	struct ifcond_mode {
		char type[20];
		ifcond_func_t func;
	};
	struct ifcond_mode ifcond_modes[] = {
		{"ifcond_signed", ifcond_signed},
		{"ifcond_unsign", ifcond_unsign},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent = 0;
	ifcond_func_t func;
	int x, y, lmt;
	int in_range;

	for (fidx = 0; fidx < SIZEOF_ARRAY (ifcond_modes); fidx++) {
		func = ifcond_modes[fidx].func;
		type = ifcond_modes[fidx].type;
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 10000000; i++)
				in_range = func (x, y, lmt);
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
