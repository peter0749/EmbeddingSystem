#!/bin/bash

mkdir src # make source dir
cd src

# Download binutils and uncompress it
wget "ftp://ftp.gnu.org/gnu/binutils/binutils-2.29.1.tar.gz"
tar zxvf "binutils-2.29.1.tar.gz"

# Clone GCC from svn
svn co "svn://gcc.gnu.org/svn/gcc/tags/gcc_4_9_3_release" gcc

# Clone Linux kernel from raspberrypi
# The Linux kernel version on my RPi is 4.14.22+
# So we clone the corresponding brach
git clone -b "rpi-4.14.y" --single-branch --depth 1 https://github.com/raspberrypi/linux

# Download GLIBC and uncompress it
wget "ftp://ftp.gnu.org/gnu/glibc/glibc-2.19.tar.gz"
tar zxvf "glibc-2.19.tar.gz"

