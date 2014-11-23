#! /bin/bash -xeu

pwd

$TEST_BINARY --from_image ${srcdir}/image.bin --split

for f in 2014*.tcx; do
  xmllint --schema ${srcdir}/TrainingCenterDatabasev2.xsd --noout $f
done

