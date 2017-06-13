
#include "optimus.h"

typedef struct {
	char name[20];
	int n;
/*} sa_t;*/
	} __attribute__((__packed__)) sa_t;

typedef struct {
	char name[20];
	int n1:1;
	int n2:1;
	int n3:1;
	int rsv:29;
/*} sb_t;*/
	} __attribute__((__packed__)) sb_t;

sa_t sa;
sb_t sb;

static inline void structbits_field (void)
{
	sb.n1 = 1;
	sb.n2 = 0;
	sb.n3 = 1;
}

static inline void structbits_bits (void)
{
	sa.n = 1 | (1 << 2);
}

int test_structbits (uint8_t argc, char **argv)
{
	typedef void (*structbits_func_t) (void);
	struct structbits_mode {
		char type[20];
		structbits_func_t func;
	};
	struct structbits_mode structbits_modes[] = {
		{"structbits_field", structbits_field},
		{"structbits_bits", structbits_bits},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	structbits_func_t func;

	for (fidx = 0; fidx < SIZEOF_ARRAY (structbits_modes); fidx++) {
		func = structbits_modes[fidx].func;
		type = structbits_modes[fidx].type;
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

