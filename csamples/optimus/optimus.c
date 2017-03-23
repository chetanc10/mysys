
#include "optimus.h"

const char *usage_str = "Usage: ./optimus <test> [additional-args]\n" \
						 "Options:\n" \
						 "\tcircinc <start> <wrap-limit>\n" \
						 "\tminmax <num> <min> <max>\n" \
						 "\tsigunsig\n" \
						 "\tintalign\n" \
						 "\tremainder <num> <divider>\n" \
						 "\tvassign\n" \
						 "\tfunccall\n" \
						 "\tcodeskip\n" \
						 "\texitloop\n" \
						 "\tprintdelay\n" \
						 "\tabsval <num1> <num2> (does num1-num2 and give 'absolute' result)\n" \
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
		{"circinc", test_circinc},
		{"minmax", test_minmax},
		{"sigunsig", test_sigunsig},
		{"intalign", test_intalign},
		{"remainder", test_remainder},
		{"vassign", test_vassign},
		{"funccall", test_funccall},
		{"codeskip", test_codeskip},
		{"exitloop", test_exitloop},
		{"printdelay", test_printdelay},
		{"absval", test_absval},
	};

	if (argc < 2) {
		ret = -1;
		goto BADLAND;
	}

	for (i = 0; i < SIZEOF_ARRAY (tests); i++) {
		if (!strcmp (tests[i].name, argv[1])) {
			ret = tests[i].cb (argc - 2, argv + 2);
			break;
		}
	}

BADLAND:
	if (ret) printf ("ret: %d\n%s", ret, usage_str);
	return ret;
}

