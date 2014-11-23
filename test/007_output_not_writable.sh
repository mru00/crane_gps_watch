#! /bin/bash -xeu

source ${srcdir}/common.sh

! $bin --output /dir-probably-does-not-exist/005_out --from_image ${srcdir}/image.bin

