
#include "optimus.h"

static uint32_t var;

static inline void vassign_if (uint32_t new)
{
	if (!var) var = new;
}

static inline void vassign_dir (uint32_t new)
{
	var = new;
}

int test_vassign (uint8_t argc, char **argv)
{
	typedef void (*vassign_func_t) (uint32_t);
	struct vassign_mode {
		char type[20];
		vassign_func_t func;
	};
	struct vassign_mode vassign_modes[] = {
		{"vassign_ift", vassign_if},
		{"vassign_dir", vassign_dir},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	vassign_func_t func;

	for (fidx = 0; fidx < SIZEOF_ARRAY (vassign_modes); fidx++) {
		func = vassign_modes[fidx].func;
		type = vassign_modes[fidx].type;
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 10000000; i++)
				func (0);
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

