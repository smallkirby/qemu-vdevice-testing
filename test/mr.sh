#!/bin/bash

QEMUVERSION="6.1.0"
QEMUPATH="$HOME/qemu-$QEMUVERSION"
QEMUBIN="$QEMUPATH/build/qemu-system-x86_64"

filesystem="rootfs.cpio"
extracted="./extracted"

extract_filesystem() {
  mkdir $extracted 
  cd $extracted 
  cpio -idv < "../$filesystem"
  cd ../
}

# extract filesystem if not exists
! [ -d "./extracted" ] && extract_filesystem

# compress
rm $filesystem 
chmod 777 -R $extracted
cd $extracted
find ./ -print0 | cpio --owner root --null -o -H newc > ../rootfs.cpio
cd ../

# run
$QEMUBIN \
  -kernel ./bzImage \
  -initrd ./rootfs.cpio \
  -nographic \
  -cpu kvm64,smep,smap \
  -append "root=/dev/ram console=ttyS0 nokaslr" \
  -device skb-simplest \
  -no-reboot \
  -s \
  -m 256M
