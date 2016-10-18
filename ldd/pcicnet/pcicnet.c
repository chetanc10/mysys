
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/mod_devicetable.h>

#include <linux/net.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/netdev_features.h>
#include <linux/etherdevice.h>
#include <linux/if_ether.h>
#include <linux/u64_stats_sync.h>

#define MODULENAME "pcicnet"

/* If a real xmit is going to happen, this shall be 1, else 0 (dummy) */
#define XMIT_IS_REAL 0

#define CNET_NAPI_WEIGHT 1

static uint64_t base_addr[6];
static uint64_t base_len[6];

struct cnet_stats {
	uint64_t packets;
	uint64_t errors;
	uint64_t dropped;
	uint64_t bytes;
	struct u64_stats_sync syncp;
} cnet_stats;

struct cnet_pcpustats {
	struct cnet_stats tx_stats;
	struct cnet_stats rx_stats;
};

struct cnet_pvt {
	struct  cnet_pcpustats cpstats;
	struct  napi_struct napi;
	struct  net_device *ndev;
	void    __iomem *ioaddr;
	uint8_t *rx_buf_ptrs[256];
	uint8_t *tx_buf_ptrs[256];
};

int (*cnet_xmit_mac_fn) (struct sk_buff *skb);

static int cnet_ndo_open (struct net_device	*ndev)
{
	struct cnet_pvt *cpvt = netdev_priv (ndev);
	printk (" cnet_ndo_open\n");
	napi_enable (&cpvt->napi);
    return 0;
}

static int cnet_ndo_close (struct net_device *ndev)
{
	int ret = 0;
	struct cnet_pvt *cpvt = netdev_priv (ndev);
	printk (" cnet_ndo_close\n");
	napi_disable (&cpvt->napi);
    return ret;
}

static netdev_tx_t cnet_start_xmit (struct sk_buff *skb, struct net_device *ndev)
{
	int ret = 0, txst = 0;
	struct cnet_pcpustats *cpstats =
		&((struct cnet_pvt *)netdev_priv (ndev))->cpstats;

    if (!skb) return (netdev_tx_t)0;

#if XMIT_IS_REAL
	if (cnet_xmit_mac_fn) {
		ret = cnet_xmit_mac_fn (skb);
		if (ret < 0) {/*check for -1/-2,etc err cases*/
			printk ("Error cnet_xmit_mac");
			txst = ret;
			ret = NETDEV_TX_BUSY;
		}
	}
#endif

	/* We may have to see if we need a work-scheduling/sync
	 * In case we do, lot of changes follow this line -TODO */
	if (!ret) { /* tx success */
		cpstats->tx_stats.packets += 1;
		cpstats->tx_stats.bytes += skb->len;
	} else { /* tx failed */
		switch (txst) {
			case -1: cpstats->tx_stats.dropped += 1; break;
			case -2: cpstats->tx_stats.errors += 1; break;
			default: printk ("What is this??\n"); break;
		}
	}

	dev_kfree_skb (skb);

    return ret;
}

static struct rtnl_link_stats64 *cnet_get_stats64 (struct net_device *ndev, struct rtnl_link_stats64 *showstats)
{
	uint32_t start;
	struct cnet_pvt *cpvt = netdev_priv (ndev);

	printk ("cnet_get_stats64\n");

	do {
		start = u64_stats_fetch_begin_irq (&cpvt->cpstats.rx_stats.syncp);
		showstats->rx_packets = cpvt->cpstats.rx_stats.packets;
		showstats->rx_bytes = cpvt->cpstats.rx_stats.bytes;
	} while (u64_stats_fetch_retry_irq (&cpvt->cpstats.rx_stats.syncp, start));

	do {
		start = u64_stats_fetch_begin_irq (&cpvt->cpstats.tx_stats.syncp);
		showstats->tx_packets = cpvt->cpstats.tx_stats.packets;
		showstats->tx_bytes = cpvt->cpstats.tx_stats.bytes;
	} while (u64_stats_fetch_retry_irq(&cpvt->cpstats.tx_stats.syncp, start));

	showstats->rx_dropped   = ndev->stats.rx_dropped;
	showstats->tx_dropped   = ndev->stats.tx_dropped;
	showstats->rx_length_errors = ndev->stats.rx_length_errors;
	showstats->rx_errors    = ndev->stats.rx_errors;
	showstats->rx_crc_errors    = ndev->stats.rx_crc_errors;
	showstats->rx_fifo_errors   = ndev->stats.rx_fifo_errors;
	showstats->rx_missed_errors = ndev->stats.rx_missed_errors;

	return showstats;
}

static const struct net_device_ops cnet_dev_ops = {
    .ndo_open         = cnet_ndo_open,
    .ndo_stop         = cnet_ndo_close,
    .ndo_start_xmit   = cnet_start_xmit,
	.ndo_get_stats64  = cnet_get_stats64,
};

static int cnet_poll (struct napi_struct *napi, int budget)
{
	int ret = 0;

	printk ("cnet_poll\n");
	return ret;
}

static struct pci_device_id pcic_dev_ids[] = {
	{0x1af4, 0x1110, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
};

static void pcic_remove (struct pci_dev *pci_dev)
{
	struct net_device *ndev = pci_get_drvdata (pci_dev);
	struct cnet_pvt *cpvt = netdev_priv (ndev);

	printk ("Removing pcic\n");

	netif_napi_del (&cpvt->napi);
    unregister_netdev (ndev);
    free_netdev (ndev);

	iounmap (cpvt->ioaddr);
	pci_release_regions (pci_dev);
	pci_clear_master (pci_dev);
	pci_disable_device (pci_dev);
}

static unsigned char pcic_mac[] = {0x34, 0x23, 0x87, 0xdf, 0x49, 0xd2};
static inline unsigned char hw_mac (int i)
{
	/* Its a dummy */
	return pcic_mac[i];
}

static int pcic_probe (struct pci_dev *pci_dev, const struct pci_device_id *id)
{
	int ret = 0;
	int i = 0;
	struct net_device *ndev = NULL;
	struct cnet_pvt *cpvt;
	void __iomem *ioaddr;

	printk ("Probing pcic\n");

    ndev = alloc_etherdev (sizeof (struct cnet_pcpustats));
    if (!ndev) {
        printk ("Failed alloc-ether-cnet-dev\n");
        return -ENOMEM;
    }

	SET_NETDEV_DEV (ndev, &pci_dev->dev);
	ndev->netdev_ops = &cnet_dev_ops;
	cpvt = netdev_priv (ndev);
	cpvt->ndev = ndev;

	/* any more net __inits? -TODO */

	/* PCI setup */
	ret = pci_enable_device (pci_dev);
	if (ret < 0) {
		printk ("ERROR pcic-enable-device\n");
		goto do_free_netdev;
	} else {
		printk ("pcic-dev-irq: %d\n", pci_dev->irq);
	}

	ret = pci_request_regions (pci_dev, MODULENAME);
	if (ret < 0) {
		printk ("ERROR pcic-request-regions\n");
		goto do_napi_del;
	}

	/* pci-dma-mask-set comes here -TODO */

	for (i = 0; i < 6; i++) {
		base_addr[i] = pci_resource_start (pci_dev, i);
		base_len[i] = pci_resource_len (pci_dev, i);
		if (base_addr[i]) {
			printk ("bar%d @ %8llx of %8llx(%llu) size\n", \
				i, base_addr[i], base_len[i], base_len[i]);
		} else {
			//break;
		}
	}

	/* ioremap here */
	ioaddr = ioremap(pci_resource_start(pci_dev, 2), \
			pci_resource_len (pci_dev, 2));
	if (!ioaddr) {
		printk ("Error ioremap-pcicnet\n");
		ret = -1;
		goto do_rel_regions;
	}

	cpvt->ioaddr = ioaddr;

	pci_set_master (pci_dev);

	/* MAC Address */
	for (i = 0; i < ETH_ALEN; i++) {
		ndev->dev_addr[i] = hw_mac (i);
	}

	netif_napi_add (ndev, &cpvt->napi, cnet_poll, CNET_NAPI_WEIGHT);

	ndev->features |= NETIF_F_RXCSUM |
		NETIF_F_HW_VLAN_CTAG_TX | NETIF_F_HW_VLAN_CTAG_RX;

    ret = register_netdev (ndev);
    if (ret < 0) {
        printk ("Failed register-netdev\n");
		goto do_napi_del;
    }

	pci_set_drvdata (pci_dev, ndev);

	return 0;

do_napi_del:
	netif_napi_del (&cpvt->napi);
	iounmap (cpvt->ioaddr);
do_rel_regions:
	pci_release_regions (pci_dev);
/*do_pci_dev_dis:*/
	pci_disable_device (pci_dev);
do_free_netdev:
	free_netdev (ndev);

	return ret;
}

static struct pci_driver pcic_drv = {
	.name = MODULENAME,
	.probe = pcic_probe,
	.remove = pcic_remove,
	.id_table = pcic_dev_ids,
};

static __init int init_pcic (void)
{
	int ret = 0;
	printk ("Initing pcic\n");
	ret = pci_register_driver (&pcic_drv);
	if (ret < 0) {
		printk ("ERROR pcic-drv-registration\n");
	}
	return ret;
}

static __exit void exit_pcic (void)
{
	printk ("Exiting pcic\n");
	pci_unregister_driver (&pcic_drv);
}

module_init (init_pcic);
module_exit (exit_pcic)
