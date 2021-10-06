# QEMU virtual device playground

# 🚧 UNDER CONSTRUCTION 🚧 

**Chicken or Egg? Development or Exploit..?**

## Description

Here are some example of QEMU virtual PCI devices.

## Contents

Refer to [device dir](./device/README.md).

## Start testing

```start.sh
cd $HOME && wget https://download.qemu.org/qemu-6.1.0.tar.xz
tar xvf qemu-6.1.0.tar.xz
git clone https://github.com/smallkirby/qemu-vdevice-testing.git
cd ./qemu-vdevice-testing

# Edit build-tree here #

./deploy.sh
```

To build your virtual device, you have to make it be included in build-tree. QEMU uses kconfig build system, but (I think) there is no GUI setting. So just apply below patch.

```patch-build.sh
### Add this in `/hw/misc/Kconfig
config SKB_SIMPLEST
    bool
    default y
    depends on PCI && MSI_NONBROKEN

### Add this in `/hw/misc/meason.build`
softmmu_ss.add(when: 'CONFIG_SKB_SIMPLEST', if_true: files('skb-simplest.c'))

### Add this in `/targets/x86_64-softmmu-config-devices.mak`
CONFIG_SKB_SIMPLEST=y
```

## Env

Tested only on Linux(Ubuntu 20.04), QEMU v6.1.0, Linux v5.10.25(as Guest).

