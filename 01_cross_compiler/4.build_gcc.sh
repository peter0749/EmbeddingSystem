#!/bin/bash

# TARGET=arm-linux-gnueabihf
# PREFIX="$PWD/crossgcc1"

cd src/gcc
rm -rf build # remove previous build
mkdir build
cd build
../configure \
    --prefix="$PREFIX" \
    --target="$TARGET" \
    --enable-languages=c,c++ --without-headers \
    --disable-libmudflap --disable-libatomic \
    --with-arch=armv6 --disable-shared \
    --enable-static --disable-decimal-float \
    --disable-libgomp --disable-libitm \
    --disable-libquadmath --disable-libsanitizer \
    --disable-libssp --disable-threads \
    --with-float=hard --with-fpu=vfp
### This may take a looooonnnnnnggggg timeeeee!!! ###
make -j
make install

