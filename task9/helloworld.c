#include <linux/module.h>
#include <linux/poll.h>
#include <linux/errno.h>
#include <linux/kobject.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>
#include <linux/string.h>

#define MY_ID "7c1caf2f50d1\n"
#define MY_ID_LEN 13		/* MY_ID length */

static DEFINE_SEMAPHORE(foo_sem);
static char foo_data[PAGE_SIZE];
static int foo_len;

static struct kobject *hello_kobj;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	down(&foo_sem);
	strncpy(buf, foo_data, foo_len);
	up(&foo_sem);

	return foo_len;
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	if (count > PAGE_SIZE)
		return -EINVAL;

	down(&foo_sem);
	strncpy(foo_data, buf, count);
	foo_len = count;
	up(&foo_sem);

	return count;
}

static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr,
		       char *buf)
{
	char *hello_str = MY_ID;

	strncpy(buf, hello_str, MY_ID_LEN);
	return MY_ID_LEN;
}

static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t count)
{
	char *hello_str = MY_ID;

	if ((count != MY_ID_LEN) || (strncmp(hello_str, buf, MY_ID_LEN)))
		return -EINVAL;
	else
		return MY_ID_LEN;
}

static ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *attr,
			    char *buf)
{
	return sprintf(buf, "%lu\n", jiffies);
}

static ssize_t jiffies_store(struct kobject *kobj, struct kobj_attribute *attr,
			     const char *buf, size_t count)
{
	return -EINVAL;
}

static struct kobj_attribute foo_attribute =
__ATTR(foo, 0644, foo_show, foo_store);
static struct kobj_attribute id_attribute = __ATTR(id, 0666, id_show, id_store);
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
MODULE_DESCRIPTION("Sysfs module");
