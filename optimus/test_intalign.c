
#include "optimus.h"

static inline int intalign_32bit (void)
{
	uint32_t i, nb_pkts = 32;

	for (i = 0; i < nb_pkts; i++) {
		printf ("C\r");
	}

	return 0;
}

static inline int intalign_16bit (void)
{
	uint16_t i, nb_pkts = 32;

	for (i = 0; i < nb_pkts; i++) {
		printf ("C\r");
	}

	return 0;
}

static inline int intalign_mnbit (void)
{
	uint16_t i;
	uint32_t nb_pkts = 32;

	for (i = 0; i < nb_pkts; i++) {
		printf ("C\r");
	}

	return 0;
}

int test_intalign (uint8_t argc, char **argv)
{
	typedef int (*intalign_func_t)(void);
	struct intalign_mode {
		char type[20];
		intalign_func_t func;
	};
	struct intalign_mode intalign_modes[] = {
		{"intalign_32bit", intalign_32bit},
		{"intalign_16bit", intalign_16bit},
		{"intalign_mnbit", intalign_mnbit},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	intalign_func_t func;

	for (fidx = 0; fidx < SIZEOF_ARRAY (intalign_modes); fidx++) {
		func = intalign_modes[fidx].func;
		type = intalign_modes[fidx].type;
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 100; i++)
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
