
#include "optimus.h"

static uint32_t lmt_mask, lmt, x;

static inline uint32_t quotient_bit (void)
{
	return x >> (lmt-1);
}

static inline uint32_t quotient_div (void)
{
	return x / lmt;
}

int test_quotient (uint8_t argc, char **argv)
{
	typedef uint32_t (*quotient_func_t) (void);
	struct quotient_dive {
		char type[20];
		quotient_func_t func;
	};
	struct quotient_dive quotient_dives[] = {
		{"quotient_bit", quotient_bit},
		{"quotient_div", quotient_div},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	quotient_func_t func;
	uint32_t quotient;

	if (argc != 2) {
		return -3;
	}

	lmt = atoi (argv[1]);
	if (lmt & (lmt - 1)) {
		printf ("test_quotient needs divisor as power of 2 only!\n");
		return -4;
	} else {
		lmt_mask = lmt - 1;
	}

	for (fidx = 0; fidx < SIZEOF_ARRAY (quotient_dives); fidx++) {
		func = quotient_dives[fidx].func;
		type = quotient_dives[fidx].type;
		x = atoi (argv[0]);
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 100000000; i++)
				quotient = func ();
			end = clock ();
			time_spent[j] = (end - start);
		}
		total_time_spent = 0;
		for (j = 0; j < 10; j++)
			total_time_spent += time_spent[j];
		printf ("clocks taken for [%s]: %f\n", type, total_time_spent/10);
	}

	if (quotient) quotient = 0; /*remove unused variable warning*/

	return 0;
}

