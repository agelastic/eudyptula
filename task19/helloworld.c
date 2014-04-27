#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/string.h>

static unsigned int myhook(const struct nf_hook_ops *ops,
			   struct sk_buff *skb,
			   const struct net_device *in,
			   const struct net_device *out,
			   int (*okfn)(struct sk_buff *))
{
	unsigned char *my_id = "7c1caf2f50d1";
	unsigned char *data;
	struct tcphdr *tcph;
	struct iphdr *iph;

	if (skb) {
		iph = (struct iphdr *)skb_network_header(skb);

		if (iph->protocol == 6) {
			tcph = (struct tcphdr *)skb_transport_header(skb);
			data = ((unsigned char *)tcph + (tcph->doff * 4));

			if (strnstr(data, my_id, skb_tail_pointer(skb) - data))
				pr_debug("Sniffed my ID: %s\n", my_id);
		}
	}

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
