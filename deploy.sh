#!/bin/bash

CUR=$(pwd)
QEMUVERSION="6.1.0"
QEMUNAME="qemu-$QEMUVERSION"
QEMUPATH="$HOME/$QEMUNAME"

BR_VERSION="2021.02.1"
BR_NAME="$HOME/$BR_VERSION"
KERNEL_VERSION="5.10.25"
KERNEL_NAME="$BR_NAME/output/build/linux-$KERNEL_VERSION"

TARGET="skb-simplest"

#######################################################

## build QEMU and virtual device.
cp "./src/$TARGET/device/$TARGET.c" "$QEMUPATH/hw/misc/$TARGET.c"
#cp "$TARGET" "$QEMUPATH/hw/misc/$BASE"
cd "$QEMUPATH" || exit
#./configure --target-list="x86_64-softmmu" --enable-debug
time ccache make -j6
cd "$CUR" || exit

echo "current: $CUR"

## build driver
cd "./src/$TARGET/driver" || exit
time make -j6

cd "$CUR" || exit