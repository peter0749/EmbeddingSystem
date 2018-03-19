#!/bin/bash
# TARGET=arm-linux-gnueabihf
# SYSROOT="$PWD/sysroot"
# PREFIX="$PWD/crossgcc2" # target path to metal binutils
cd src/binutils-* # enter whatever binutils dir
rm -rf build_cx "$PREFIX" # remove old build
mkdir build_cx # isolate build files from source codes (keep tarball clean)
cd build_cx
../configure --prefix="$PREFIX"  --target="$TARGET" \
    --with-sysroot="$SYSROOT"
### WARNING: PLEASE CHECK YOUR C_INCLUDE_PATH IS SET CORRECTLY! ###
make -j
make install

