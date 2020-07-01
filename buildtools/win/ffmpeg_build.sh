#！/bin/bash

# --target-os win32 win64 linux android
# 

function build_ffmpeg
{
    echo "Start to build ffmpeg for $ARCH"
    ./configure --target-os=win32 \
    --prefix=$PREFIX \
    --arch=$ARCH \
    --disable-doc \
    --disable-shared \
    --enable-static \
    --disable-yasm \
    --disable-asm \
    --disable-symver \
    --enable-gpl 

    make clean
    make
    make install
    echo "Build finished."
}

echo "Begin to build ffmpeg."
ARCH=x86
PREFIX=$(pwd)/ffmpeg_build_dir/win32/

build_ffmpeg

sleep 20
