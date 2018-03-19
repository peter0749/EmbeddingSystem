# SYSROOT="$PWD/sysroot"
cd src/linux
make headers_install ARCH=arm \
    INSTALL_HDR_PATH="$SYSROOT/usr"

