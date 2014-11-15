#! /bin/bash -xeu

pwd

../src/crane_gps_watch_client --out output1.tcx --from_image image.bin --to_image image2.bin
../src/crane_gps_watch_client --out output2.tcx --from_image image2.bin


xmllint --schema TrainingCenterDatabasev2.xsd --noout output1.tcx
xmllint --schema TrainingCenterDatabasev2.xsd --noout output2.tcx

test -e output.tcx
