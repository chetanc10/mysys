
#include <linux/module.h>
#include <linux/net.h>
#include <linux/netdevice.h>

struct net_device *cnet_dev;

static int cnet_ndo_open (struct net_device *dev)
{
    return 0;
}

static int cnet_ndo_close (struct net_device *dev)
{
    return 0;
}

static netdev_tx_t cnet_start_xmit (struct sk_buff *skb, struct net_device *net_dev)
{
    if (skb) dev_kfree_skb (skb);
    return NETDEV_TX_OK;
}

static const struct net_device_ops cnet_dev_ops = {
    .ndo_open = cnet_ndo_open,
    .ndo_stop = cnet_ndo_close,
    .ndo_start_xmit = cnet_start_xmit,
};

static void cnet_do_init(struct net_device *net)
{
    net->watchdog_timeo = 2 * HZ;
}

static int __init cnet_init (void)
{
    int ret = 0;
    printk ("Initing cnet\n");

    cnet_dev = alloc_netdev (0, "ceth0", cnet_do_init);
    if (!cnet_dev) {
        printk ("Failed alloc-ether-cnet-dev\n");
        return -1;
    }
    cnet_dev->netdev_ops = &cnet_dev_ops;

    ret = register_netdev (cnet_dev);
    if (ret < 0) {
        printk ("Failed register-netdev\n");
        free_netdev (cnet_dev);
    }

    return ret;
}

static void __exit cnet_exit (void)
{
    printk ("Exiting cnet\n");
    unregister_netdev (cnet_dev);
    free_netdev (cnet_dev);
}

module_init (cnet_init);
module_exit (cnet_exit);
