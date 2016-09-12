#include <linux/init.h>
#include <linux/module.h>
#include <linux/time.h>

MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
	/*printk(KERN_ALERT "Hello, world\n");*/
	unsigned long get_time;
	struct timeval tv;
	struct tm tv2;

	do_gettimeofday(&tv);
	get_time = tv.tv_sec;
	time_to_tm(get_time, 0, &tv2);
	printk(" @ UTC time (%04d-%02d-%02d %02d:%02d:%02d)\n", tv2.tm_year + 1900, tv2.tm_mon + 1, tv2.tm_mday, tv2.tm_hour, tv2.tm_min, tv2.tm_sec);

	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
