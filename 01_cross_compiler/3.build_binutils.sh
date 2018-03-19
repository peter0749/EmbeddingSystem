#!/bin/bash
# TARGET=arm-linux-gnueabihf
# PREFIX="$PWD/crossgcc1" # target path to metal binutils
cd src/binutils-* # enter whatever binutils dir
rm -rf build "$PREFIX" # remove old build
mkdir build # isolate build files from source codes (keep tarball clean)
cd build
../configure --prefix="$PREFIX"  --target="$TARGET"
### WARNING: PLEASE CHECK YOUR C_INCLUDE_PATH IS SET CORRECTLY! ###
make -j
make install

