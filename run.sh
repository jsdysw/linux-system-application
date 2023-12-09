#!/bin/bash

sudo insmod pxt4/jbd3/jbd3.ko
sudo insmod pxt4/pxt4.ko
sudo mount -t pxt4 /dev/nvme0n1 /mnt/test
sudo fio write.fio
sudo rm -rf /mnt/test/fio-test.*
sudo umount /mnt/test
sudo rmmod pxt4
sudo rmmod jbd3
