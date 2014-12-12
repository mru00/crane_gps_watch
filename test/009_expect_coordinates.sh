#! /bin/bash -eux

source ${srcdir}/common.sh

ln -s ${srcdir_abs}/knedlyk_fail.bin

ls ${srcdir}

ls 


expect_exit 0 $bin --from_image knedlyk_fail.bin --verbose | tee 009_log

expect_exit 0 grep ' workout end samples with gps=9002' 009_log

