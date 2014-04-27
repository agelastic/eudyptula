#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

static unsigned int myhook(const struct nf_hook_ops *ops,
			   struct sk_buff *skb,
			   const struct net_device *in,
			   const struct net_device *out,
			   int (*okfn)(struct sk_buff *)) 
{
	return NF_ACCEPT;
}

static struct nf_hook_ops euhooks =  {
	.hook = myhook,
	.hooknum = NF_INET_LOCAL_IN,
	.pf = NFPROTO_IPV4,
	.priority = NF_INE_PRI_FIRST
};

static int __init hello_init(void)
{
	return nf_register_hook(&euhooks);
}

static void __exit hello_exit(void)
{
	nf_unregister_hook(&euhooks);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("7c1caf2f50d1");
MODULE_DESCRIPTION("netfilter module task 19");
