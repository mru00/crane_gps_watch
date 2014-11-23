#! /bin/bash -xeu

pwd

$TEST_BINARY --out 001_output.tcx --from_image ${srcdir}/image.bin

xmllint --schema ${srcdir}/TrainingCenterDatabasev2.xsd --noout 001_output.tcx

test -e 001_output.tcx
