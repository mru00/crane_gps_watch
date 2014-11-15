#! /bin/bash -xeu

pwd

${srcdir}/crane_gps_watch_client --out output.tcx --from_image image.bin

xmllint --schema TrainingCenterDatabasev2.xsd --noout output.tcx

test -e output.tcx
