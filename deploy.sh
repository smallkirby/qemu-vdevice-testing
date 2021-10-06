#!/bin/bash

PWD=$(pwd)
QEMUVERSION="6.1.0"
QEMUNAME="qemu-$QEMUVERSION"
QEMUPATH="$HOME/$QEMUNAME"

BASE="skb.c"
TARGET="./device/skb-simplest.c"

#######################################################

cp "$TARGET" "$QEMUPATH/hw/misc/$(basename -- $TARGET)"
#cp "$TARGET" "$QEMUPATH/hw/misc/$BASE"
cd "$QEMUPATH" || exit
#./configure --target-list="x86_64-softmmu" --enable-debug
time ccache make -j6
cd "$PWD" || exit
