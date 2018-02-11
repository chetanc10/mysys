#include<stdio.h>
#include<ctype.h>
#include<stdint.h>
#include<time.h>

struct timespec start,stop,begin,end;

int main()
{
	char a[5] = {'a','b','c','d'};
	uint64_t time=0;

	clock_gettime(CLOCK_REALTIME, &start);
	if((a[0] >= 'a' && a[0] <= 'z') || \
		(a[0] >= 'A' && a[0] <= 'Z'));
	clock_gettime(CLOCK_REALTIME, &stop);
	time = ((1000000000 * (stop.tv_sec - start.tv_sec)) + (stop.tv_nsec - start.tv_nsec));
	printf("timetaken:%lu\n",time);

	time =0;
	clock_gettime(CLOCK_REALTIME, &begin);
	if(isalpha(a[0]));
	clock_gettime(CLOCK_REALTIME, &end);
	time = ((1000000000 * (end.tv_sec - begin.tv_sec)) + (end.tv_nsec - begin.tv_nsec));
	printf("timetaken:%lu\n",time);
	return 0;
}
