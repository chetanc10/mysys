
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/mod_devicetable.h>

static uint64_t base_addr[6];
static uint64_t base_len[6];

static struct pci_device_id pcic_dev_ids[] = {
	{0x1af4, 0x1110, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0},
};

static void pcic_remove (struct pci_dev *dev)
{
	printk ("Removing pcic\n");
	pci_clear_master (dev);
	pci_disable_device (dev);
}

static int pcic_probe(struct pci_dev *dev, const struct pci_device_id *id)
{
	int ret = 0;
	int i = 0;

	printk ("Probing pcic\n");

	ret = pci_enable_device (dev);
	if (ret < 0) {
		printk ("ERROR pcic-enable-device\n");
	} else {
		printk ("pcic-dev-irq: %d\n", dev->irq);
	}

	pci_set_master (dev);

	for (i = 0; i < 6; i++) {
		base_addr[i] = pci_resource_start (dev, i);
		base_len[i] = pci_resource_len (dev, i);
		if (base_addr[i]) {
			printk ("bar%d @ %016lx of %016lx(%lu) size\n", \
				i, base_addr[i], base_len[i], base_len[i]);
		} else {
			//break;
		}
	}
	return ret;
}

static struct pci_driver pcic_drv = {
	.name = "pcic_drv",
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
