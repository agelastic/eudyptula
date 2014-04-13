/*
* helloworld.c
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>

struct identity {
	char name[20];
	int id;
	bool busy;
	struct list_head list; 
};

static LIST_HEAD(identity_list);

int identity_create(char *name, int id)
{
//TODO
}

struct identity *identity_find(int id)
{
//TODO
}

int *identity_destroy(int id)
{
//TODO
}

static int hello_init(void)
{
	struct identity *temp;

	pr_debug("Hello World!\n");
	identity_create("Alice", 1);
	identity_create("Bob", 2);
	identity_create("Dave", 3);
	identity_create("Gena", 10);

	temp = identity_find(3);
	pr_debug("id 3 = %s\n", temp->name);

	temp = identity_find(42);
	if (temp == NULL)
		pr_debug("id 42 not found\n");

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42); /* this should return an error */
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
