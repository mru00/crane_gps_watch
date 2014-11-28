#! /bin/bash -xeu
# Copyright (C) 2014 mru@sisyphus.teil.cc

source ${srcdir}/common.sh

expect_exit 0 $bin --out 002_output1.tcx --from_image ${srcdir_abs}/image.bin --to_image 002_image2.bin
expect_exit 0 $bin --out 002_output2.tcx --from_image 002_image2.bin


xmllint --schema ${srcdir_abs}/TrainingCenterDatabasev2.xsd --noout 002_output1.tcx
xmllint --schema ${srcdir_abs}/TrainingCenterDatabasev2.xsd --noout 002_output2.tcx

