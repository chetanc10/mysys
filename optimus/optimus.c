
#include "optimus.h"
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>

const char *usage_str = "Usage: ./optimus <test> [additional-args]\n"
						 "Options:\n"
						 "\tcircinc <start> <wrap-limit>\n"
						 "\tminmax <num> <min> <max>\n"
						 "\tsigunsig\n"
						 "\tintalign\n"
						 "\tremainder <num> <divider>\n"
						 "\tvassign\n"
						 "\tfunccall\n"
						 "\tcodeskip\n"
						 "\texitloop\n"
						 "\tprintdelay\n"
						 "\tabsval <num1> <num2> (does num1-num2 and give 'absolute' result)\n"
						 "\tstructbits\n"
						 "\tbitset\n"
						 "\tcondret\n"
						 "\tzerofy\n"
						 "\tquotient\n"
						 "\tmcopy\n"
						 "\tcache\n"
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
		{"structbits", test_structbits},
		{"bitset", test_bitset},
		{"condret", test_condret},
		{"zerofy", test_zerofy},
		{"quotient", test_quotient},
		{"mcopy", test_mcopy},
		{"cache", test_cache},
	};

	if (argc < 2) {
		ret = -1;
		goto BADLAND;
	}

	for (i = 0; i < SIZEOF_ARRAY (tests); i++) {
		if (!strcmp (tests[i].name, argv[1])) {
			cpu_set_t cs;
			int ncpus = get_nprocs ();
			CPU_ZERO (&cs);
			(ncpus > 1) ? (--ncpus) : (ncpus);
			CPU_SET (ncpus, &cs);
			if (0 > sched_setaffinity (getpid (), sizeof (cs), &cs)) {
				printf ("optimus %d affinity failed. Reason: %s\n", ncpus, strerror (errno));
				return -errno;
			} else {
				char scall[64];
				printf ("new affinity: %d\n", ncpus);
				sprintf (scall, "sudo renice -n %d %u", -19, getpid ());
				if (0 != system (scall)) {
					perror ("renice: ");
				}
			}
			ret = tests[i].cb (argc - 2, argv + 2);
			break;
		}
	}

BADLAND:
	if (ret) printf ("ret: %d\n%s", ret, usage_str);
	return ret;
}

