#! /bin/bash -xeu

source ${srcdir}/common.sh


for img in ${srcdir_abs}/lint_images/*.bin ${srcdir_abs}/*.bin; do

  echo -e '\n\n\n'
  $bin --lint --verbose --out 008_output.tcx --from_image $img
  xmllint --schema ${srcdir_abs}/TrainingCenterDatabasev2.xsd --noout 008_output.tcx
done

