#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

char str[] = "c0:c1:c0:c9:8b:5f, Cisco26776                      , 01,  INFRA , -74 , WPA2-PERSONAL\n06:00:12:f0:00:01, vvdnchennai                     , 01,  INFRA , -77 , WPA2-PERSONAL\nf8:52:df:14:00:83, SHYAM                           , 01,  INFRA , -76 , NONE\n00:1e:e5:7d:7e:3d, aptina                          , 06,  INFRA , -61 , WPA2-PERSONAL\n00:1f:33:27:a9:a0, BTFU_testing                    , 11,  INFRA , -84 , WPA2-PERSONAL";

int main(int argc, char **argv)
{
	/*double startTime, endTime;*/
	char a[64] = "";
	char *ptr = str;
	/*char nulls[32] = "";*/
	/*char a1[32] = "", a2[32] = "";*/

	// 	startTime = getCPUTime( );

#if 1
	sscanf (ptr, "%[^,]", a);
	ptr += strlen(a);
	printf ("mac  : %s;\r\n", a);
	sscanf (ptr+2, "%[^ ]", a);
	ptr += strlen(a);
	printf ("ssid : %s;\r\n", a);
#else
#if 0
	sscanf (ptr, "%[^,]%[^ ] %[^ ]", a, a2, a1);
	printf ("%s;%c;%s;\r\n", a, *a2, a1);
#else
	sscanf (ptr, "%[^,], %[^ ]", a, a1);
	printf ("%s;%c;%s;\r\n", a, a1);
#endif
#endif
#if 0
	sscanf (ptr, "%[^,]", nulls);
	ptr += strlen(nulls);
#else
	ptr = strchr (ptr, ',');
#endif
	// 	printf ("ptr  : %s;\r\n", ptr);

	// 	endTime = getCPUTime( );

	// 	sleep (2);
	// 	fprintf( stderr, "CPU time used = %lf\n", (endTime - startTime) );

	return 0;
}
