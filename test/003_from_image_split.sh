#! /bin/bash -xeu

source ${srcdir}/common.sh

ln -s ${srcdir_abs}/image.bin

expect_exit 0 $bin --from_image image.bin --split --verbose


expected_files=( 2014-11-03T10_50_56+01_00.tcx 2014-11-03T22_44_03+01_00.tcx )
actual_files=( $(ls *.tcx | sort) )

expect_exit 0 test "${actual_files[*]}" = "${expected_files[*]}"


for f in ${actual_files[@]}; do
  xmllint --schema ${srcdir_abs}/TrainingCenterDatabasev2.xsd --noout $f
done

expect_lap_count 2014-11-03T22_44_03+01_00.tcx 1

