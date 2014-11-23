#! /bin/bash -eu

$TEST_BINARY --from_image $srcdir/knedlyk_fail.bin --verbose > 009_log

grep ' workout end samples with gps=9002' 009_log

