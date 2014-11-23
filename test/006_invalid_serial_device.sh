#! /bin/bash -xeu

source ${srcdir}/common.sh

! $bin --output 006_out --device file-probably-does-not-exist

! $bin --output 006_out --device ${srcdir}/image.bin
