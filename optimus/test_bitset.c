
#include "optimus.h"

static inline uint32_t bitset_shift (uint32_t n)
{
	uint32_t i = 0, shift = 1;

    while (1) {
        while (n >> shift) {
            n >>= shift;
            i += shift;
            shift <<= 1;
            if (n & 1) return i;
        }
        shift >>= 1;
    }
}

static inline uint32_t bitset_incr (uint32_t n)
{
	uint32_t i = 0, shift = 1;

    while (1) {
        while (n >> shift) {
            n >>= shift;
            i += shift;
            shift += 1;
            if (n & 1) return i;
        }
        shift -= 1;
    }
}

static inline uint32_t bitset_simp (uint32_t n)
{
	uint32_t i = 0;

	while (n >> 1) {
		n >>= 1;
		i += 1;
		if (n & 1) return i;
	}

	return 0;
}

int test_bitset (uint8_t argc, char **argv)
{
	typedef uint32_t (*bitset_func_t) (uint32_t);
	struct bitset_mode {
		char type[20];
		bitset_func_t func;
	};
	struct bitset_mode bitset_modes[] = {
		{"bitset_shift", bitset_shift},
		{"bitset_incr", bitset_incr},
		{"bitset_simp", bitset_simp},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	bitset_func_t func;
	uint32_t n, pos;

	if (argc != 1) {
		return -3;
	}

	n = atoi (argv[0]);

	for (fidx = 0; fidx < SIZEOF_ARRAY (bitset_modes); fidx++) {
		func = bitset_modes[fidx].func;
		type = bitset_modes[fidx].type;
		for (j = 0; j < 10; j++) {
			start = clock ();
			for (i = 0; i < 100000; i++)
				pos = func (n);
			end = clock ();
			time_spent[j] = (end - start);
		}
		total_time_spent = 0;
		for (j = 0; j < 10; j++)
			total_time_spent += time_spent[j];
		printf ("clocks taken for [%s]: %f\n", type, total_time_spent/10);
	}

	if (pos < 1) return 0;

	return 0;
}

