
#include "optimus.h"

static inline void printdelay_full (void)
{
	printf ("debug log in the file %s with line number %d\n", __func__, __LINE__);
	printf ("error log in code file %s\n", __FILE__);
}

static inline void printdelay_enc (void)
{
	printf ("dbg:%s,%d\n", __func__, __LINE__);
	printf ("err:%s\n", __FILE__);
}

int test_printdelay (uint8_t argc, char **argv)
{
	typedef void (*printdelay_func_t) (void);
	struct printdelay_mode {
		char type[20];
		printdelay_func_t func;
	};
	struct printdelay_mode printdelay_modes[] = {
		{"printdelay_full", printdelay_full},
		{"printdelay_enc", printdelay_enc},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	printdelay_func_t func;

	for (fidx = 0; fidx < SIZEOF_ARRAY (printdelay_modes); fidx++) {
		func = printdelay_modes[fidx].func;
		type = printdelay_modes[fidx].type;
		for (j = 0; j < 5; j++) {
			start = clock ();
			for (i = 0; i < 5; i++)
				func ();
			end = clock ();
			time_spent[j] = (end - start);
		}
		total_time_spent = 0;
		for (j = 0; j < 5; j++)
			total_time_spent += time_spent[j];
		printf ("clocks taken for [%s]: %f\n", type, total_time_spent/5);
	}

	return 0;
}

