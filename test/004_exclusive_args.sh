#! /bin/bash -xeu

pwd

! ../src/crane_gps_watch_client --output 004_test.output --split --from_image ${srcdir}/image.bin
! test -e 004_test.output


! ../src/crane_gps_watch_client --split --output 004_test.output --from_image ${srcdir}/image.bin
! test -e 004_test.output



! ../src/crane_gps_watch_client --output 004_test.output --device /devicefile --from_image ${srcdir}/image.bin
! test -e 004_test.output



! ../src/crane_gps_watch_client --output 004_test.output --from_image ${srcdir}/image.bin --device /devicefile
! test -e 004_test.output
