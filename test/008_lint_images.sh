#! /bin/bash -xeu

for img in ${srcdir}/lint_images/*.bin ${srcdir}/*.bin; do

  echo -e '\n\n\n'
  ../src/crane_gps_watch_client --lint --verbose --out 008_output.tcx --from_image $img
  xmllint --schema ${srcdir}/TrainingCenterDatabasev2.xsd --noout 008_output.tcx
done

