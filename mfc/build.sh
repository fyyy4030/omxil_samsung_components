#!/bin/sh

echo $TOOLCHAIN_PATH
echo $OMX_INSTALL_PATH
echo $OMXCORE_INCLUDE_PATH

export PKG_CONFIG_LIBDIR=

cflags=" -I/usr/include/arm-linux-gnueabihf \
        -I$OMX_INSTALL_PATH/include \
        -I$TOOLCHAIN_PATH/include \
	-I$OMXCORE_INCLUDE_PATH \
	-L$OMX_INSTALL_PATH/usr/lib \
	$OMX_CFLAGS"

make distclean
autoreconf -i

PKG_CONFIG_PATH=$OMX_PKG_CONFIG_PATH \
ac_cv_func_memset_0_nonnull=yes ac_cv_func_realloc_0_nonnull=yes  ac_cv_func_malloc_0_nonnull=yes CFLAGS="$cflags" ./configure \
 --host=arm-linux-gnueabi \
 --prefix=$OMX_INSTALL_PATH/usr \
 --enable-shared \
 --includedir=$OMX_INSTALL_PATH/usr/include
#  CFLAGS="$cflags -DCFG_DEBUG_LEVEL=255"


mkdir -p $OMX_INSTALL_PATH

make uninstall #DESTDIR=$OMX_INSTALL_PATH
make
make install #DESTDIR=$OMX_INSTALL_PATH
