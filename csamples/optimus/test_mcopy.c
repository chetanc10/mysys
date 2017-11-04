
#include "optimus.h"

struct mystruct {
	int n;
	char c[12];
	long l;
};

static struct mystruct s1, s2;

static inline void mcopy_memcpy (void)
{
	memcpy (&s2, &s1, sizeof (struct mystruct));
}

static inline void mcopy_assign (void)
{
	s2 = s1;
}

int test_mcopy (uint8_t argc, char **argv)
{
	typedef void (*mcopy_func_t) (void);
	struct mcopy_mode {
		char type[20];
		mcopy_func_t func;
	};
	struct mcopy_mode mcopy_modes[] = {
		{"mcopy_assign", mcopy_assign},
		{"mcopy_memcpy", mcopy_memcpy},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	mcopy_func_t func;

	for (fidx = 0; fidx < SIZEOF_ARRAY (mcopy_modes); fidx++) {
		func = mcopy_modes[fidx].func;
		type = mcopy_modes[fidx].type;
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 10000000; i++)
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

