
#include "optimus.h"

typedef void (*func_t) (uint32_t n);

func_t f_arr[5];

void func1 (uint32_t n)
{
	n <<= 1;
	n <<= 1;
	n <<= 1;
}

void func2 (uint32_t n)
{
	n <<= 2;
	n <<= 2;
	n <<= 2;
}

void func3 (uint32_t n)
{
	n <<= 3;
	n <<= 3;
	n <<= 3;
}

void func4 (uint32_t n)
{
	n <<= 4;
	n <<= 4;
	n <<= 4;
}

void func5 (uint32_t n)
{
	n <<= 5;
	n <<= 5;
	n <<= 5;
}

func_t farr[5] = {
	func1, func2, func3, func4, func5
};

static inline void funccall_swt (uint32_t funcIdx)
{
	switch (funcIdx) {
		case 1: func1 (funcIdx); break;
		case 2: func2 (funcIdx); break;
		case 3: func3 (funcIdx); break;
		case 4: func4 (funcIdx); break;
		case 5: func5 (funcIdx); break;
		default: break;
	}
}

static inline void funccall_arr (uint32_t funcIdx)
{
	farr[funcIdx] (funcIdx);
}

static inline void funccall_ift (uint32_t funcIdx)
{
	if (funcIdx == 1) func1 (funcIdx);
	else if (funcIdx == 2) func2 (funcIdx);
	else if (funcIdx == 3) func3 (funcIdx);
	else if (funcIdx == 4) func4 (funcIdx);
	else if (funcIdx == 5) func5 (funcIdx);
}

int test_funccall (uint8_t argc, char **argv)
{
	typedef void (*funccall_func_t) (uint32_t);
	struct funccall_mode {
		char type[20];
		funccall_func_t func;
	};
	struct funccall_mode funccall_modes[] = {
		{"funccall_ift", funccall_ift},
		{"funccall_swt", funccall_swt},
		{"funccall_arr", funccall_arr},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	funccall_func_t func;

	for (fidx = 0; fidx < SIZEOF_ARRAY (funccall_modes); fidx++) {
		func = funccall_modes[fidx].func;
		type = funccall_modes[fidx].type;
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 1000000; i++)
				func (4);
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

