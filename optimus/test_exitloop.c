
#include "optimus.h"

static inline uint32_t exitloop_break (void)
{
	uint32_t i;
	uint32_t temp1, temp2;
	for (i = 0; i < 10; i++) {
		if (i == 5)
			break;
		temp1 = (((1 << i) % 32 + i) - 1) && 0x000000ff;
		temp2 = (temp1 < 32) ? temp1 : 32;
	}
	return temp2;
}

static inline uint32_t exitloop_goto (void)
{
	uint32_t i;
	uint32_t temp1, temp2;
	for (i = 0; i < 10; i++) {
		if (i == 5)
			goto out_of_loop;
		temp1 = (((1 << i) % 32 + i) - 1) && 0x000000ff;
		temp2 = (temp1 < 32) ? temp1 : 32;
	}
out_of_loop:
	return temp2;
}

int test_exitloop (uint8_t argc, char **argv)
{
	typedef uint32_t (*exitloop_func_t) (void);
	struct exitloop_mode {
		char type[20];
		exitloop_func_t func;
	};
	struct exitloop_mode exitloop_modes[] = {
		{"exitloop_break", exitloop_break},
		{"exitloop_goto", exitloop_goto},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	exitloop_func_t func;

	for (fidx = 0; fidx < SIZEOF_ARRAY (exitloop_modes); fidx++) {
		func = exitloop_modes[fidx].func;
		type = exitloop_modes[fidx].type;
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 1000000; i++)
				func ();
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

