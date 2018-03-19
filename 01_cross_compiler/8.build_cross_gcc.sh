#!/bin/bash

# TARGET=arm-linux-gnueabihf
# SYSROOT="$PWD/sysroot"
# PREFIX="$PWD/crossgcc2"

cd src/gcc
rm -rf build_cx # remove previous build
mkdir build_cx
cd build_cx
../configure \
    --prefix="$PREFIX" \
    --target="$TARGET" \
    --enable-languages=c,c++ \
    --with-sysroot="$SYSROOT" \
    --with-arch=armv6  \
    --with-float=hard --with-fpu=vfp \
    --disable-libmudflap --enable-libgomp \
    --disable-libssp --enable-libquadmath \
    --enable-libquadmath-support \
    --disable-libsanitizer --enable-lto \
    --enable-threads=posix --enable-target-optspace \
    --with-linker-hash-style=gnu --disable-nls \
    --disable-multilib --enable-long-long
### This may take a looooonnnnnnggggg timeeeee!!! ###
make -j
make install

# echo "export PATH=$PREFIX/bin:\$PATH" >> "$HOME/.bashrc" # export path to toolchain to global scope
