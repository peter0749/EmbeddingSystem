#!/bin/bash
git clone git://git.busybox.net/buildroot
cd buildroot
make raspberrypi3_defconfig
make source
# make menuconfig
make 

