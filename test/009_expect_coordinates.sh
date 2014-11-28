#! /bin/bash -eu

source ${srcdir}/common.sh


expect_exit 0 $bin --from_image ${srcdir_abs}/knedlyk_fail.bin --verbose > 009_log

expect_exit 0 grep ' workout end samples with gps=9002' 009_log

