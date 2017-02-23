
#include "optimus.h"

static inline uint32_t codeskip_cont (void)
{
	uint32_t i;
	uint32_t temp1;
	for (i = 0; i < 10; i++) {
		if (i == 5)
			continue;
		temp1 = (((1 << i) % 32 + i) - 1) && 0x000000ff;
		(temp1 < 32) ? temp1 : 32;
	}
	return 0;
}

static inline uint32_t codeskip_goto (void)
{
	uint32_t i;
	uint32_t temp1;
	for (i = 0; i < 10;) {
		if (i == 5)
			goto update_i;
		temp1 = (((1 << i) % 32 + i) - 1) && 0x000000ff;
		(temp1 < 32) ? temp1 : 32;
update_i:
		i++;
	}
	return 0;
}

int test_codeskip (uint8_t argc, char **argv)
{
	typedef uint32_t (*codeskip_func_t) (void);
	struct codeskip_mode {
		char type[20];
		codeskip_func_t func;
	};
	struct codeskip_mode codeskip_modes[] = {
		{"codeskip_cont", codeskip_cont},
		{"codeskip_goto", codeskip_goto},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	codeskip_func_t func;

	for (fidx = 0; fidx < SIZEOF_ARRAY (codeskip_modes); fidx++) {
		func = codeskip_modes[fidx].func;
		type = codeskip_modes[fidx].type;
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

