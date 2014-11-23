#! /bin/bash -xeu

source ${srcdir}/common.sh

! $bin --output 004_test.output --split --from_image ${srcdir}/image.bin
! test -e 004_test.output


! $bin --split --output 004_test.output --from_image ${srcdir}/image.bin
! test -e 004_test.output


! $bin --output 004_test.output --device /devicefile --from_image ${srcdir}/image.bin
! test -e 004_test.output


! $bin --output 004_test.output --from_image ${srcdir}/image.bin --device /devicefile
! test -e 004_test.output
