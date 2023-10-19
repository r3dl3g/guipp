#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"/..

export RASPBIAN_BASE=/opt/raspberrypi
export RASPBIAN_ROOTFS=$RASPBIAN_BASE/rootfs
export PATH=/opt/raspberrypi/cross-pi-gcc/bin:/opt/raspberrypi/cross-pi-gcc/libexec/gcc/arm-linux-gnueabihf/8.3.0:$PATH
export RASPBERRY_VERSION=1

BUILD_DIR_R=$DIR/./armhf/Debug
BUILD_DIR_R=$DIR/./armhf/Release
if [ ! -d $BUILD_DIR_D ]; then
  mkdir -p $BUILD_DIR_D
fi
if [ ! -d $BUILD_DIR_R ]; then
  mkdir -p $BUILD_DIR_R
fi

pushd $BUILD_DIR_D
#cmake -DCMAKE_TOOLCHAIN_FILE=$RASPBIAN_BASE/raspi-toolchain/Toolchain-rpi.cmake -DBUILD_FOR_ARM=ON -DCMAKE_BUILD_TYPE=Debug $DIR
popd

pushd $BUILD_DIR_R
cmake -DCMAKE_TOOLCHAIN_FILE=$RASPBIAN_BASE/raspi-toolchain/Toolchain-rpi.cmake -DBUILD_FOR_ARM=ON -DCMAKE_BUILD_TYPE=Release $DIR
make -j10
popd
