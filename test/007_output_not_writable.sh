#! /bin/bash -xeu

source ${srcdir}/common.sh

ln -s ${srcdir_abs}/image.bin

expect_exit 1 $bin --output /dir-probably-does-not-exist/005_out --from_image image.bin

