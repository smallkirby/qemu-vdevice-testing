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

// device information
typedef struct {
  struct pci_dev *pdev;

  unsigned long pio_base, pio_flags, pio_len;
  unsigned pio_memsize;

  unsigned short vendorid, deviceid;
} skb_devinfo;

static skb_devinfo *dinfo;

// ID table of devices for this driver.
static struct pci_device_id skb_pci_ids[] = {
    {PCI_DEVICE(SKB_VENDOR_ID, SKB_DEVICE_ID)},
    { 0, },
};
MODULE_DEVICE_TABLE(pci, skb_pci_ids);

// driver constructor / destructor
static int skb_pci_probe(struct pci_dev *pdev, const struct pci_device_id *id) {
  int ret;
  pr_info("probing now.\n");

  dinfo->pdev = pdev;
  dinfo->pio_memsize = IOSKB_IOSIZE;

  // enable device
  if((ret = pci_enable_device(pdev)) != 0) {
    pr_err("failed to enable PCI.\n");
    goto error;
  };
  pr_info("enabled PCI.\n");

  // get info of PIO
  dinfo->pio_base = pci_resource_start(pdev, SKB_BAR_PIO);
  dinfo->pio_len = pci_resource_len(pdev, SKB_BAR_PIO);
  dinfo->pio_flags = pci_resource_flags(pdev, SKB_BAR_PIO);
  pr_info("infomation PIO >\n");
  pr_info("  base: 0x%016lx, len: 0x%lx, flags: %lx\n", dinfo->pio_base, dinfo->pio_len, dinfo->pio_flags);
  if(!(dinfo->pio_flags & IORESOURCE_IO)) {
    pr_err("BAR0 is not for PIO.\n");
    ret = -1;
    goto error;
  }

  // reserve PCI PIO region. Only after successful allocation, we can touch the PCI region(including vendor-id).
  if((ret = pci_request_region(pdev, SKB_BAR_PIO, TYPE_PCI_SKB_DEV)) != 0) {
    pr_err("failed to request PIO region.\n");
    goto error;
  }

  pci_read_config_word(pdev, PCI_VENDOR_ID, &dinfo->vendorid);
  pci_read_config_word(pdev, PCI_DEVICE_ID, &dinfo->deviceid);
  pr_info("  Vendor: 0x%x, Device: 0x%x\n", dinfo->vendorid, dinfo->deviceid);

  pr_info("successfully proved dev.\n");

error:
  return ret;
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
  dinfo = kmalloc(sizeof(skb_devinfo), GFP_KERNEL);
  if (IS_ERR_OR_NULL(dinfo)) {
    pr_err("failed to alloc memory for skb_devinfo.\n");
    return -1;
  }
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
