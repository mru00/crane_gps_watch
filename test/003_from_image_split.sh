#! /bin/bash -xeu

source ${srcdir}/common.sh

expect_exit 0 $bin --from_image ${srcdir_abs}/image.bin --split

for f in *.tcx; do
  xmllint --schema ${srcdir_abs}/TrainingCenterDatabasev2.xsd --noout $f
done

