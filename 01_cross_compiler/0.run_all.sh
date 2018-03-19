#!/bin/bash

TARGET=arm-linux-gnueabihf
CROSS_GCC1_PREFIX="$PWD/crossgcc1"
CROSS_GCC2_PREFIX="$PWD/crossgcc2"
SYSROOT="$PWD/sysroot"
STDLOG="$PWD/logs_std"
ERRLOG="$PWD/logs_err"

./1.install_requirements.sh || ( >&2 echo "Failed to run 1." && exit )
./2.download_tarballs.sh || ( >&2 echo "Failed to run 2." && exit)
TARGET=${TARGET} PREFIX=${CROSS_GCC1_PREFIX} ./3.build_binutils.sh || ( >&2 echo "Failed to run 3." && exit)
TARGET=${TARGET} PREFIX=${CROSS_GCC1_PREFIX} \
    PATH="$CROSS_GCC1_PREFIX/bin:$PATH" ./4.build_gcc.sh || ( >&2 echo "Failed to run 4." && exit)
SYSROOT=${SYSROOT} ./5.install_kernel_headers.sh || ( >&2 echo "Failed to run 5." && exit)
PATH="$CROSS_GCC1_PREFIX/bin:$PATH" SYSROOT=${SYSROOT} \
    HOST=${TARGET} TARGET=${TARGET} ./6.build_glibc.sh || ( >&2 echo "Failed to run 6." && exit)
PATH="$CROSS_GCC1_PREFIX/bin:$PATH" SYSROOT=${SYSROOT} \
    TARGET=${TARGET} PREFIX=${CROSS_GCC2_PREFIX} ./7.build_cross_binutils.sh || ( >&2 echo "Failed to run 7." && exit)
PATH="$CROSS_GCC2_PREFIX/bin:$PATH" SYSROOT=${SYSROOT} \
    TARGET=${TARGET} PREFIX=${CROSS_GCC2_PREFIX} ./8.build_cross_gcc.sh || ( >&2 echo "Failed to run 8." && exit)

"${CROSS_GCC2_PREFIX}/bin/${TARGET}-gcc" "$PWD/zj_d799.c" -o "$PWD/zj_d799" || ( >&2 echo "Failed to compile source code!" )

