#! /bin/bash -xeu

source ${srcdir}/common.sh

expect_exit 0 $bin --out 001_output.tcx --from_image ${srcdir_abs}/image.bin

expect_exit 0 xmllint --schema ${srcdir_abs}/TrainingCenterDatabasev2.xsd --noout 001_output.tcx

test -e 001_output.tcx
