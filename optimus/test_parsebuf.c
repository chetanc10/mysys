
#include "optimus.h"

int pos;
char *found = NULL;
char str[10000] = "Taking	 advantage	 of	 the	 collapse	 of	 the	 Mughal	 empire	 and	 the	 rise	 of	 a	 number	 of \
		warring	principalities	contending	for	authority	across	eighteenth-century	India,	the	British \
		had	subjugated	a	vast	land	through	the	power	of	their	artillery	and	the	cynicism	of	their \
		amorality.	They	displaced	nawabs	and	maharajas	for	a	price,	emptied	their	treasuries	as	it \
		pleased	them,	took	over	their	states	through	various	methods	(including,	from	the	1840s, \
				the	cynical	‘doctrine	of	lapse’	whenever	a	ruler	died	without	an	heir),	and	stripped	farmers \
		of	their	ownership	of	the	lands	they	had	tilled	for	generations.	With	the	absorption	of	each \
		native	state,	the	Company	official	John	Sullivan	(better	known	as	the	founder	of	the	‘hill- \
				station’	 of	 Ootacamund,	 or	 ‘Ooty’,	 today	 known	 more	 correctly	 as	 Udhagamandalam) \
		observed	in	the	1840s:	‘The	little	court	disappears—trade	languishes—the	capital	decays \
		—the	 people	 are	 impoverished—the	 Englishman	 flourishes,	 and	 acts	 like	 a	 sponge, \
		drawing	 up	 riches	 from	 the	 banks	 of	 the	 Ganges,	 and	 squeezing	 them	 down	 upon	 the \
			banks	of	the	Thames.’";
char idx[1000] = "without an  heir) Englishman";

static inline void parsebuf_direct (void)
{
	found = strstr (str, "Englishman");
}

static inline void parsebuf_index (void)
{
	pos = *(uint16_t *)((char *)strstr (idx, "Englishman") + strlen ("Englishman"));
	found = str + pos;
}

int test_parsebuf (uint8_t argc, char **argv)
{
	typedef void (*parsebuf_func_t)(void);
	struct parsebuf_mode {
		char type[20];
		parsebuf_func_t func;
	};
	struct parsebuf_mode parsebuf_modes[] = {
		{"parsebuf_indexx", parsebuf_index},
		{"parsebuf_direct", parsebuf_direct},
	};
	uint32_t i, j, fidx;
	char *type;
	clock_t start, end;
	double time_spent[10], total_time_spent;
	parsebuf_func_t func;

	for (fidx = 0; fidx < SIZEOF_ARRAY (parsebuf_modes); fidx++) {
		func = parsebuf_modes[fidx].func;
		type = parsebuf_modes[fidx].type;
		if (strcmp ("parsebuf_index", type)) {
			*(uint16_t *)(idx + strlen(idx)) = (uint16_t)(strstr (str, "Englishman") - str);
		}
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
		printf ("clocks taken for [%s]: %f\n", \
				type, total_time_spent/10);
	}

	return 0;
}
