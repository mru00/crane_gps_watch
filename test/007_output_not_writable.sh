#! /bin/bash -xeu

source ${srcdir}/common.sh

expect_exit 1 $bin --output /dir-probably-does-not-exist/005_out --from_image ${srcdir_abs}/image.bin

