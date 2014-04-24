#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/poll.h>
#include <linux/errno.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/sched.h>

#define MY_ID "7c1caf2f50d1"
#define MY_ID_LEN 13	/* ID length including the final NULL */

DECLARE_WAIT_QUEUE_HEAD(wee_wait);

static struct task_struct *eudy_thread;

static int thread_main(void *data)
{
	while (1) {
		if (wait_event_interruptible(wee_wait, kthread_should_stop()))
			return -ERESTARTSYS;

		if (kthread_should_stop())
			break;
	}

	return 0;
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
	.write = hello_write
};

static struct miscdevice hello_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "eudyptula",
	.fops = &hello_fops,
	.mode = 0222
};

static int __init hello_init(void)
{
	char *mythread = "eudyptula";

	eudy_thread = kthread_run(thread_main, NULL, mythread);
	if (eudy_thread == ERR_PTR(-ENOMEM))
		return -ENOMEM;

	return misc_register(&hello_dev);
}

static void __exit hello_exit(void)
{
	kthread_stop(eudy_thread);
	misc_deregister(&hello_dev);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("7c1caf2f50d1");
MODULE_DESCRIPTION("Task 17 module");
