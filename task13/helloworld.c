/*
* helloworld.c
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/string.h>
#include <linux/err.h>
#include <linux/slab.h>

#define NAME_LEN 20

struct identity {
	char name[NAME_LEN];
	int id;
	bool busy;
	struct list_head list;
};

static LIST_HEAD(identity_list);

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
	int retval = -EINVAL;

	if (identity_find(id))
		goto out;

	temp = kmalloc(sizeof(*temp), GFP_KERNEL);
	if (!temp)
		goto out;

	strncpy(temp->name, name, NAME_LEN);
	temp->name[NAME_LEN-1] = '\0';
	temp->id = id;
	temp->busy = false;
	list_add(&(temp->list), &identity_list);
	retval = 0;

	pr_debug("identity %d: %s created\n", id, name);

out:	return retval;

}

void identity_destroy(int id)
{
	struct identity *temp;

	temp = identity_find(id);

	if (!temp)
		return;

	pr_debug("destroying identity %i: %s\n", temp->id, temp->name);

	list_del(&(temp->list));
	kfree(temp);

	return;
}

static int hello_init(void)
{
	struct identity *temp;

	pr_debug("Hello World!\n");

	if (identity_create("Alice", 1))
		pr_debug("error creating Alice\n");
	if (identity_create("Bob", 2))
		pr_debug("error creating Bob\n");
	if (identity_create("Dave", 3))
		pr_debug("error creating Dave\n");
	if (identity_create("Gena", 10))
		pr_debug("error creating Gena\n");

	temp = identity_find(3);
	pr_debug("id 3 = %s\n", temp->name);

	temp = identity_find(42);
	if (temp == NULL)
		pr_debug("id 42 not found\n");

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);

	return 0;
}

static void hello_exit(void)
{
	pr_debug("See you later.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("7c1caf2f50d1");
MODULE_DESCRIPTION("Just a module");
