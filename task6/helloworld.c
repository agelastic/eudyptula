#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/errno.h>

#define MY_ID "7c1caf2f50d1"
#define MY_ID_LEN 13	/* ID length including the final NULL */

static ssize_t hello_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	char *hello_str = MY_ID;

	if (*ppos != 0)
		return 0;

	if ((count < MY_ID_LEN) ||
		(copy_to_user(buf, hello_str, MY_ID_LEN)))
		return -EINVAL;

	*ppos += count;
	return count;
}

static ssize_t hello_write(struct file *file, char const __user *buf,
			size_t count, loff_t *ppos)
{
	char *hello_str = MY_ID;
	char input[MY_ID_LEN];

	if ((count != MY_ID_LEN) ||
		(copy_from_user(input, buf, MY_ID_LEN)) ||
		(strncmp(hello_str, input, MY_ID_LEN - 1)))
		return -EINVAL;
	else
		return count;
}

static const struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.read = hello_read,
	.write = hello_write
};

static struct miscdevice hello_dev = {
	MISC_DYNAMIC_MINOR,
	"eudyptula",
	&hello_fops
};

static int hello_init(void)
{
	int ret;

	ret = misc_register(&hello_dev);
	pr_debug("Hello World!\n");

	return ret;
}

static void hello_exit(void)
{
	misc_deregister(&hello_dev);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("7c1caf2f50d1");
MODULE_DESCRIPTION("Misc char hello world module");
