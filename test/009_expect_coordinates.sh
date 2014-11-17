#! /bin/bash -eu

../src/crane_gps_watch_client --from_image $srcdir/knedlyk_fail.bin --verbose > 009_log

grep ' workout end, samples with coord: 9000' 009_log

