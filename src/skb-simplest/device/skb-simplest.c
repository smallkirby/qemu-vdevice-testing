#include "qemu/osdep.h"
#include "hw/hw.h"
#include "hw/pci/pci.h"
#include "hw/qdev-properties.h"
#include "qemu/event_notifier.h"
#include "qemu/module.h"
#include "sysemu/kvm.h"
#include "qom/object.h"

#define TYPE_PCI_SKB_DEV        "skb-simplest"
#define SKB_DEVICE_ID           0x1337
#define IOSKB_IOSIZE            0x80      // pio memory size
#define SKB_BAR_PIO             0x0

// virtual PCI device structure
typedef struct SkbState {
  PCIDevice pdev;

  MemoryRegion pio;
  char piodata[IOSKB_IOSIZE];
} SkbState;

OBJECT_DECLARE_SIMPLE_TYPE(SkbState, PCI_SKB_DEV)

// port I/O read handler
static uint64_t skb_pci_pio_read(void *opaque, hwaddr addr, unsigned size) {
  printf("[SKB] skb_pci_read() called.\n");
  SkbState *s = opaque;

  if (addr < IOSKB_IOSIZE) {
    return s->piodata[addr];
  }
  else
  {
    return 0xdeadbeef;
  }

  return 0;
}

// port I/O handlers
static const MemoryRegionOps skb_pci_pio_ops = {
    .read = skb_pci_pio_read,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .impl = {
        .min_access_size = 1,
        .max_access_size = 1,
    },
};

// initialization of instance
static void pci_skb_realize(PCIDevice *pci_dev, Error **errp) {
  SkbState *skb = PCI_SKB_DEV(pci_dev);
  uint8_t *pci_conf = pci_dev->config;
  printf("[SKB] skb_instance_init() is called.\n");

  // set interrupt off
  pci_conf[PCI_INTERRUPT_PIN] = 0;
  // init PIO region
  memory_region_init_io(&skb->pio, OBJECT(skb), &skb_pci_pio_ops, skb, "pic_skb_portio", IOSKB_IOSIZE * 2);
  // register PIO entry in BAR
  pci_register_bar(pci_dev, SKB_BAR_PIO, PCI_BASE_ADDRESS_SPACE_IO, &skb->pio);
  // init PIO data region
  memset(skb->piodata, 0, IOSKB_IOSIZE);
  strcpy(skb->piodata, "Hello, world! Can you hear me?\x00");
}

// destructor of instance
static void pci_skb_uninit(PCIDevice *dev) {
  SkbState *s = PCI_SKB_DEV(dev);

  printf("[SKB] pci_skb_uninit() called.\n");
}

// initialization of class
static void skb_class_init(ObjectClass *class, void *data) {
  DeviceClass *dc = DEVICE_CLASS(class);
  PCIDeviceClass *k = PCI_DEVICE_CLASS(class);

  k->realize = pci_skb_realize;
  k->exit = pci_skb_uninit;
  k->vendor_id = PCI_VENDOR_ID_QEMU;
  k->device_id = SKB_DEVICE_ID;
  k->revision = 0;
  k->class_id = PCI_CLASS_OTHERS;
  dc->desc = "Sample Port I/O PCI device for skb.";
  set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

// registration of typeinfo
static void pci_skb_register_types(void) {
    static InterfaceInfo interfaces[] = {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    };
    static const TypeInfo skb_info = {
        .name          = TYPE_PCI_SKB_DEV,
        .parent        = TYPE_PCI_DEVICE,
        .instance_size = sizeof(SkbState),
        .class_init    = skb_class_init,
        .interfaces = interfaces,
    };

    type_register_static(&skb_info);
}
type_init(pci_skb_register_types)
