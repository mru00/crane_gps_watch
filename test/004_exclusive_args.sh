#! /bin/bash -xeu

source ${srcdir}/common.sh

expect_exit 1 $bin --output 004_test.output --split --from_image ${srcdir_abs}/image.bin
! test -e 004_test.output


expect_exit 1 $bin --split --output 004_test.output --from_image ${srcdir_abs}/image.bin
! test -e 004_test.output


expect_exit 1 $bin --output 004_test.output --device /devicefile --from_image ${srcdir_abs}/image.bin
! test -e 004_test.output


expect_exit 1 $bin --output 004_test.output --from_image ${srcdir_abs}/image.bin --device /devicefile
! test -e 004_test.output
