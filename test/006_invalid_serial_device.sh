#! /bin/bash -xeu

source ${srcdir}/common.sh

ln -s ${srcdir_abs}/image.bin

expect_exit 1 $bin --output 006_out --device file-probably-does-not-exist

expect_exit 1 $bin --output 006_out --device image.bin
