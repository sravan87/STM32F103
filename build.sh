#!/bin/bash

CURDIR=`pwd`
TOOLCHAIN_PATH=$CURDIR/gcc-arm-none-eabi-4_9-2015q3/bin/
PATH=$PATH:$TOOLCHAIN_PATH

export FREE_RTOS_SUPPORTED=1

make $@



