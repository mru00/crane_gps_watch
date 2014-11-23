#! /bin/bash -xeu

source ${srcdir}/common.sh

$bin --out 001_output.tcx --from_image ${srcdir_abs}/image.bin

xmllint --schema ${srcdir_abs}/TrainingCenterDatabasev2.xsd --noout 001_output.tcx

test -e 001_output.tcx
