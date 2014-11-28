#! /bin/bash -xeu

source ${srcdir}/common.sh

expect_exit 1 $bin --output 005_out --from_image file-probably-does-not-exist

