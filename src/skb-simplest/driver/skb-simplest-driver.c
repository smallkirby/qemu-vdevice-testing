#include<linux/errno.h>
#include<linux/file.h>
#include<linux/fs.h>
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/init.h>
#include<linux/types.h>
#include<linux/kernel.h>
#include<linux/cdev.h>
#include<linux/sched.h>
#include<linux/slab.h>
#include<linux/device.h>
#include<linux/pci.h>
#include<linux/interrupt.h>
#include<asm/dma.h>
#include<asm/current.h>
#include<asm/uaccess.h>

#define pr_fmt(fmt) "%s:%s(): " fmt, KBUILD_MODNAME, __func__

#define TYPE_PCI_SKB_DEV        "skb-simplest"
#define SKB_DEVICE_ID           0x1337
#define SKB_VENDOR_ID           0x1234
#define IOSKB_IOSIZE            0x80      // pio memory size
#define SKB_BAR_PIO             0x0


static const struct file_operations skb_simplest_fops = {
	.owner = THIS_MODULE,
};

// ID table of devices for this driver.
static struct pci_device_id skb_pci_ids[] = {
    {PCI_DEVICE(SKB_VENDOR_ID, SKB_DEVICE_ID)},
    { 0, },
};
MODULE_DEVICE_TABLE(pci, skb_pci_ids);

// driver constructor / destructor
static int skb_pci_probe(struct pci_dev *pdev, const struct pci_device_id *id) {
  pr_info("probing now.\n");
  return 0;
}

static void skb_pci_remove(struct pci_dev *pdev) {
  pr_info("removing now.\n");
}

// driver itself
static struct pci_driver skb_pci_driver = {
    .name = TYPE_PCI_SKB_DEV,
    .id_table = skb_pci_ids,
    .probe = skb_pci_probe,
    .remove = skb_pci_remove,
};

// constructor / destructor
static int __init skb_simplest_init(void)
{
  return pci_register_driver(&skb_pci_driver);
}

static void __exit skb_simplest_exit(void)
{
  return pci_unregister_driver(&skb_pci_driver);
}

module_init(skb_simplest_init);
module_exit(skb_simplest_exit);
MODULE_AUTHOR("NIRUGIRI");
MODULE_LICENSE("GPL");
