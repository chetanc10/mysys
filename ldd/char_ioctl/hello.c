
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ChetaN");
MODULE_DESCRIPTION("Event Manager for Linux Userspace applications");

#define CHARDEV_NAME "hello"

#define HELLO_MAGIC 'k'

#define HELLO_IOC_KMALLOC _IO(HELLO_MAGIC, 0)

int err;
int major;
int dev_opened;

struct hello_dev {
	struct cdev cdev;
} hd;

static int hello_open (struct inode *inode, struct file *file)
{
	if (dev_opened) {
		printk (KERN_WARNING "Already opened!!\n");
		return -EINVAL;
	}

	dev_opened = 1;
	printk (KERN_INFO "hello_open success\n");

	return 0;
}

static int hello_release (struct inode *inode, struct file *file)
{
	if (!dev_opened) {
		printk (KERN_WARNING "Already closed!!\n");
		return -EINVAL;
	}

	dev_opened = 0;
	printk (KERN_INFO "hello_close success\n");

	return 0;
}

static ssize_t hello_read (struct file *file, char __user *u, size_t count, loff_t *ppos)
{
	printk ("hello_read\n");
	return count;
}

static ssize_t hello_write (struct file *file, const char __user *u, size_t count, loff_t *ppos)
{
	printk ("hello_write\n");
	return count;
}

static ssize_t hello_ioctl (struct file *filp, unsigned int cmd, unsigned long args)
{
	/*int ret = 0;*/

	printk (KERN_INFO "ioctl hello\n");
	switch (cmd) {
		case HELLO_IOC_KMALLOC: printk (KERN_INFO "hello_ioctl cmd: %d\n", cmd); return 0;
		default: return -1;
	}
}

static struct file_operations fops = {
	.read = hello_read,
	.write = hello_write,
	.open = hello_open,
	.unlocked_ioctl = hello_ioctl,
	.release = hello_release,
};

static int __init hello_init(void)
{
	int devno, dev;
	int err;
    printk(KERN_INFO "Hello world!\n");

	err = alloc_chrdev_region( &devno , 0, 1,"hello");
	if (err < 0) {
		printk (KERN_ERR "alloc_chardev_region failed with %d\n", err);
		return err;
	}

	major = MAJOR (devno);
	dev = MKDEV (major, 0);
	
	cdev_init (&hd.cdev, &fops);
	hd.cdev.owner = THIS_MODULE;

	err = cdev_add (&hd.cdev, dev, 1);
	if (err < 0) {
		printk (KERN_ERR "Registering the character device failed with %d\n", err);
		return err;
	}

	printk (KERN_INFO "Registered\n");

    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
	cdev_del (&hd.cdev);
}

module_init(hello_init);
module_exit(hello_cleanup);

