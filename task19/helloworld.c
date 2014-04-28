#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/textsearch.h>

#define MY_ID "7c1caf2f50d1"

static unsigned int myhook(const struct nf_hook_ops *ops,
			   struct sk_buff *skb,
			   const struct net_device *in,
			   const struct net_device *out,
			   int (*okfn)(struct sk_buff *))
{
	int pos;
	struct ts_state state;
	struct ts_config *conf;

	conf = textsearch_prepare("kmp", MY_ID, 12, GFP_KERNEL, TS_AUTOLOAD);
	if (IS_ERR(conf))
		return NF_ACCEPT;

	for (pos = skb_find_text(skb, 0, INT_MAX, conf, &state);
	     pos != UINT_MAX;
	     pos = textsearch_next(conf, &state))
		pr_debug(MY_ID " at %d\n", pos);

	textsearch_destroy(conf);
	return NF_ACCEPT;
}

static struct nf_hook_ops euhooks =  {
	.hook = myhook,
	.hooknum = NF_INET_LOCAL_IN,
	.pf = NFPROTO_IPV4,
	.priority = NF_IP_PRI_FIRST
};

static int __init hello_init(void)
{
	pr_debug("Registering hook\n");
	return nf_register_hook(&euhooks);
}

static void __exit hello_exit(void)
{
	pr_debug("Unregistering hook\n");
	nf_unregister_hook(&euhooks);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("7c1caf2f50d1");
MODULE_DESCRIPTION("netfilter module task 19");
