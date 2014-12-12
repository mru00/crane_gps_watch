#! /bin/bash -xeu

source ${srcdir}/common.sh

ln -s ${srcdir_abs}/image.bin

expect_exit 0 $bin --from_image image.bin --split

for f in *.tcx; do
  xmllint --schema ${srcdir_abs}/TrainingCenterDatabasev2.xsd --noout $f
done

