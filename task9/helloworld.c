#include <linux/module.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/errno.h>
#include <linux/kobject.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>

#define MY_ID "7c1caf2f50d1\n"
#define MY_ID_LEN 13	/* MY_ID length */

static DEFINE_SEMAPHORE(foo_sem);
static char foo_data[PAGE_SIZE];
static int foo_len;

static struct kobject *hello_kobj;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{//FIXME
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t count)
{
//FIXME
}

static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{//FIXME
}

static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t count)
{
//FIXME
}
static ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{//FIXME
}

static ssize_t jiffies_store(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t count)
{
//FIXME
}

static struct kobj_attribute foo_attribute = 
	__ATTR(foo, 0644, foo_show, foo_store);
static struct kobj_attribute id_attribute = 
	__ATTR(id, 0666, id_show, id_store);
static struct kobj_attribute jiffies_attribute = 
	__ATTR(jiffies, 0444, jiffies_show, jiffies_store);

static struct attribute *attrs[] = {
	&foo_attribute.attr,
	&id_attribute.attr,
	&jiffies_attribute.attr,
	NULL,
};

static struct attribute_group hello_group = {
	.attrs = attrs,
};


/*
static ssize_t foo_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	int retval = -EINVAL;

	if (*ppos != 0)
		return 0;

	down(&foo_sem);

	if (!copy_to_user(buf, foo_data, foo_len)) {
		*ppos += count;
		retval = count;
	}

	up(&foo_sem);
	return retval;
}

static ssize_t foo_write(struct file *file, char const __user *buf,
			size_t count, loff_t *ppos)
{
	int retval = -EINVAL;

	if (count >= PAGE_SIZE)
		return -EINVAL;

	down(&foo_sem);

	if (copy_from_user(foo_data, buf, count)) {
		foo_len = 0;
	} else {
		foo_len = count;
		retval = count;
	}

	up(&foo_sem);
	return retval;
}

static const struct file_operations foo_fops = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write
};

static ssize_t id_read(struct file *file, char __user *buf,
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

static ssize_t id_write(struct file *file, char const __user *buf,
			size_t count, loff_t *ppos)
{
	char *hello_str = MY_ID;
	char input[MY_ID_LEN];

	if ((count != MY_ID_LEN) ||
		(copy_from_user(input, buf, MY_ID_LEN)) ||
		(strncmp(hello_str, input, MY_ID_LEN)))
		return -EINVAL;
	else
		return count;
}

static const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.read = id_read,
	.write = id_write
};

*/
static int __init hello_init(void)
{
	int retval;

	hello_kobj = kobject_create_and_add("eudyptula", kernel_kobj);
	if (!hello_kobj)
		return -ENOMEM;

	retval = sysfs_create_group(hello_kobj, &hello_group);
	if (retval)
		kobject_put(hello_kobj);

	pr_debug("Hello World!\n");
	return retval;
}

static void __exit hello_exit(void)
{
	kobject_put(hello_kobj);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vitaly Osipov <vitaly.osipov@gmail.com>");
MODULE_DESCRIPTION("Debugfs module");
