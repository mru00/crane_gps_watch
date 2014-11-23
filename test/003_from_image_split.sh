#! /bin/bash -xeu

source ${srcdir}/common.sh

$bin --from_image ${srcdir}/image.bin --split

for f in *.tcx; do
  xmllint --schema ${srcdir}/TrainingCenterDatabasev2.xsd --noout $f
done

