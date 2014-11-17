#! /bin/bash -xeu

pwd

for img in ${srcdir}/lint_images/*.bin; do

  ../src/crane_gps_watch_client --lint --verbose --verbose --out 008_output.tcx --from_image $img
  xmllint --schema ${srcdir}/TrainingCenterDatabasev2.xsd --noout 008_output.tcx
done

