#!/bin/bash

source musl-build.sh

make ARCH=arm64 CROSS_COMPILE=aarch64-musl-linux-musl- O=build defconfig
make ARCH=arm64 CROSS_COMPILE=aarch64-musl-linux-musl- O=build -j8
