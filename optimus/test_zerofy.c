
#include "optimus.h"

typedef struct {
	uint32_t a;
	uint64_t b;
	uint64_t c;
	uint64_t d;
	char string[64];
} ss_t;

static inline void zerofy_memset (ss_t *s)
{
	memset ((void *)&(s->b), 0, sizeof (ss_t) - sizeof (s->a));
}

static inline void zerofy_assign (ss_t *s)
{
	s->b = 0;
	s->c = 0;
	s->d = 0;
	memset (s->string, 0, 64);
}

int test_zerofy (uint8_t argc, char **argv)
{
	typedef void (*zerofy_func_t) (ss_t *s);
	struct zerofy_mode {
		char type[20];
		zerofy_func_t func;
	};
	struct zerofy_mode zerofy_modes[] = {
		{"zerofy_memset", zerofy_memset},
		{"zerofy_assign", zerofy_assign},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	zerofy_func_t func;
	ss_t s;

	for (fidx = 0; fidx < SIZEOF_ARRAY (zerofy_modes); fidx++) {
		func = zerofy_modes[fidx].func;
		type = zerofy_modes[fidx].type;
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 10000000; i++)
				func (&s);
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

