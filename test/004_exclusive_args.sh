#! /bin/bash -xeu

source ${srcdir}/common.sh

ln -s ${srcdir_abs}/image.bin

expect_exit 1 $bin --output 004_test.output --split --from_image image.bin
! test -e 004_test.output


expect_exit 1 $bin --split --output 004_test.output --from_image image.bin
! test -e 004_test.output


expect_exit 1 $bin --output 004_test.output --device /devicefile --from_image image.bin
! test -e 004_test.output


expect_exit 1 $bin --output 004_test.output --from_image image.bin --device /devicefile
! test -e 004_test.output
