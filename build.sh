#!/bin/bash

export OMX_CFLAGS="-g -O0"
export TOOLCHAIN_PATH="/usr"
export OMX_INSTALL_PATH=""
export OMXCORE_INCLUDE_PATH=$OMX_INSTALL_PATH/usr/include
export OMX_PKG_CONFIG_PATH=$OMX_INSTALL_PATH/usr/lib/pkgconfig
export LIBS="-lexplain"


#if cd camera; then ./build.sh; cd ..; fi

#if cd ffmpeg; then ./build.sh ; cd ..; fi

#if cd colorconv; then ./build.sh; cd ..; fi

#if cd fbvideo; then ./build.sh; cd ..; fi

#if cd drmvideo; then ./build.sh; cd ..; fi

#if cd ffmpeg-dist; then ./build.sh ; cd ..; fi

if cd mfc; then ./build.sh ; cd ..; fi

#if cd examples; then ./build.sh ; cd ..; fi

if cd xvideo; then ./build.sh ; cd ..; fi

mv /usr/lib/*mfc* /usr/lib/bellagio/
