#!/bin/bash

# setting the device files for Reboot
# Developed by Taeklim Kim

sudo insmod ~/workspace/babycare/client_dev/led/led_dev.ko
sudo insmod ~/workspace/babycare/client_dev/pir/pir_dev.ko
sudo insmod ~/workspace/babycare/serv_dev/button_dev.ko

sudo mknod -m 666 /dev/led_dev c 240 0
sudo mknod -m 666 /dev/pir_dev c 241 0

