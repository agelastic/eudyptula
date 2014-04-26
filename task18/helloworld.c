#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/poll.h>
#include <linux/errno.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/mutex.h>

DECLARE_WAIT_QUEUE_HEAD(wee_wait);

static struct task_struct *eudy_thread;

#define NAME_LEN 20

struct identity {
	char name[NAME_LEN];
	int id;
	bool busy;
	struct list_head list;
};

static LIST_HEAD(identity_list);
static int counter;

static DEFINE_MUTEX(i_mutex);

struct identity *identity_get(void)
{
	struct identity *temp;

	if (list_empty(&identity_list))
		return NULL;

	mutex_lock(&i_mutex);
	temp = list_entry(identity_list.next, struct identity, list);
	list_del(&temp->list);
	mutex_unlock(&i_mutex);

	return temp;
}

struct identity *identity_find(int id)
{
	struct identity *temp;

	list_for_each_entry(temp, &identity_list, list) {
		if (temp->id == id)
			return temp;
	}

	return NULL;
}

int identity_create(char *name, int id)
{
	struct identity *temp;

	if (identity_find(id))
		return -EINVAL;

	temp = kmalloc(sizeof(*temp), GFP_KERNEL);
	if (!temp)
		return -EINVAL;

	mutex_lock(&i_mutex);
	strncpy(temp->name, name, NAME_LEN-1);
	temp->name[NAME_LEN-1] = '\0';
	temp->id = id;
	temp->busy = false;
	list_add(&(temp->list), &identity_list);
	mutex_unlock(&i_mutex);

	pr_debug("Created identity: %s %i\n", temp->name, temp->id);
	return 0;

}

void identity_destroy(int id)
{
	struct identity *temp;

	temp = identity_find(id);

	if (!temp)
		return;

	list_del(&(temp->list));
	kfree(temp);

	return;
}

static int thread_main(void *data)
{
	struct identity *temp;

	while (1) {
		if (wait_event_interruptible(wee_wait, 1))
			return -ERESTARTSYS;

		if (kthread_should_stop())
			break;

		temp = identity_get();

		if (temp) {
			msleep_interruptible(5000);
			pr_debug("Got identity: %s %i\n", temp->name, temp->id);
			kfree(temp);
		}
	}
	return 0;
}

static ssize_t hello_write(struct file *file, char const __user *buf,
			size_t count, loff_t *ppos)
{
	char input[NAME_LEN];
	int real_count = (count >= NAME_LEN) ? NAME_LEN-1 : count;

	memset(input, '\0', NAME_LEN);

	if (copy_from_user(input, buf, real_count))
		return -EINVAL;

	if (identity_create(input, counter))
		return -EINVAL;

	counter++;
	wake_up(&wee_wait);
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
	struct identity *temp, *next;

	kthread_stop(eudy_thread);
	misc_deregister(&hello_dev);
	list_for_each_entry_safe(temp, next, &identity_list, list) {
		list_del(&temp->list);
		kfree(temp);
	}
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("7c1caf2f50d1");
MODULE_DESCRIPTION("Task 17 module");
