#!/bin/bash

# sudo apt-get install texinfo libncurses5-dev libx11-dev

wget ftp://ftp.gnu.org/gnu/gdb/gdb-8.1.tar.gz
tar zxvf gdb-8.1.tar.gz

rm -rf build_gdb
mkdir build_gdb
cd build_gdb

../gdb-8.1/configure --prefix=${TOOL_CHAIN_PATH} \
                     --target="arm-linux-gnueabihf" \
                     --enable-tui

make -j5
make install

