# skb-simplest

- PIO
- simple driver
  - registers the PIC device and enable the device / PIC region.
  - but no handler for IO is implemented.

Successful install would show below message:
```success.sh
/ # insmod ./skb-simplest-driver.ko
[   22.846959] skb_simplest_driver: loading out-of-tree module taints kernel.
[   22.873171] skb_simplest_driver:skb_pci_probe(): probing now.
[   22.873803] skb_simplest_driver:skb_pci_probe(): enabled PCI.
[   22.874421] skb_simplest_driver:skb_pci_probe(): infomation PIO >
[   22.874973] skb_simplest_driver:skb_pci_probe():   base: 0x000000000000c000, len: 0x100, flags: 40101
[   22.875939] skb_simplest_driver:skb_pci_probe():   Vendor: 0x1234, Device: 0x1337
[   22.876475] skb_simplest_driver:skb_pci_probe(): successfully proved dev.
[   22.893623] insmod (89) used greatest stack depth: 14352 bytes left

/ # lspci -k
00:01.0 Class 0601: 8086:7000
00:04.0 Class 00ff: 1234:1337 skb-simplest
00:00.0 Class 0600: 8086:1237
00:01.3 Class 0680: 8086:7113
00:03.0 Class 0200: 8086:100e e1000
00:01.1 Class 0101: 8086:7010 ata_piix
00:02.0 Class 0300: 1234:1111
```
