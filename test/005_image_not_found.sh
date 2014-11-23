#! /bin/bash -xeu

source ${srcdir}/common.sh

! $bin --output 005_out --from_image file-probably-does-not-exist

