# HOST=arm-linux-gnueabihf
# TARGET=$HOST
# SYSROOT="$PWD/sysroot"
cd src/glibc-*
rm -rf clean_build
mkdir clean_build
cd clean_build
### CHECK YOUR LD_LIBRARY_PATH IS CORRECT !!! ###
../configure --prefix=/usr \
    --build="x86_64-linux-gnu" \
    --host="$HOST" --target="$TARGET" \
    --with-headers="$SYSROOT/usr/include" \
    --includedir=/usr/include --enable-add-ons \
    --disable-multilib
### THIS SHOULD TAKE A LONG TIME ###
make -j
make install install_root="$SYSROOT"

