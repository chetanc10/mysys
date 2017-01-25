
#include "optimus.h"

const char *usage_str = "Usage: ./optimus <test> [additional-args]\n" \
						 "Options:\n" \
						 "\tcircinc <start> <wrap-limit>\n" \
						 "\tminmax <num> <min> <max>\n" \
						 "\tsigunsig\n" \
						 "\tintalign\n" \
						 ;

typedef int (*test_cb_t)(uint8_t argc, char **argv);

typedef struct {
	char      name[20];
	test_cb_t cb;
} test_ctxt_t;

int main (int argc, char **argv)
{
	int ret = -2;
	uint32_t i = 0;
	test_ctxt_t tests[] = {
		{"test_circinc", test_circinc},
		{"test_minmax", test_minmax},
		{"test_sigunsig", test_sigunsig},
		{"test_intalign", test_intalign},
	};

	if (argc < 2) {
		ret = -1;
		goto BADLAND;
	}

	for (i = 0; i < SIZEOF_ARRAY (tests); i++) {
		if (!strcmp (&tests[i].name[5], argv[1])) {
			ret = tests[i].cb (argc - 2, argv + 2);
			break;
		}
	}

BADLAND:
	if (ret) printf ("%s", usage_str);
	return ret;
}

