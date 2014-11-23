#! /bin/bash -eu

source ${srcdir}/common.sh


$bin --from_image ${srcdir_abs}/knedlyk_fail.bin --verbose > 009_log

grep ' workout end samples with gps=9002' 009_log

