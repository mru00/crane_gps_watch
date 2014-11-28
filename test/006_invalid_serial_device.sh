#! /bin/bash -xeu

source ${srcdir}/common.sh

expect_exit 1 $bin --output 006_out --device file-probably-does-not-exist

expect_exit 1 $bin --output 006_out --device ${srcdir_abs}/image.bin
