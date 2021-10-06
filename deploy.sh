#!/bin/bash

PWD=$(pwd)

cp ./device/skb.c ~/qemu-6.1.0/hw/misc/
cd ~/qemu-6.1.0
#./configure --target-list="x86_64-softmmu" --enable-debug
time ccache make -j6
cd $PWD
