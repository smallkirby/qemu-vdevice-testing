BR_VERSION="2021.02.1"
BR_NAME="$(HOME)/buildroot-$(BR_VERSION)"
KERNEL_VERSION="5.10.25"
KERNEL_NAME="$(BR_NAME)/output/build/linux-$(KERNEL_VERSION)"

obj-m := skb-simplest-driver.o

all:
	make -C $(KERNEL_NAME) M=$(PWD) modules

clean:
	make -C $(KERNEL_NAME) M=$(PWD) clean
