#! /bin/bash -eux

# Copyright (C) 2015 mru@sisyphus.teil.cc

source ${srcdir}/common.sh

for img in ${srcdir_abs}/lapcount*.bin; do
    ln -s $img
done


for i in 0 1 2 3; do

    $bin --verbose --from_image lapcount_${i}.bin --output tcxfile_$i | tee logfile_$i
    expect_exit 0 test $(grep -c "lap begin" logfile_$i) -eq $i

    if [[ $i -gt 0 ]]; then
        expect_lap_count tcxfile_$i $i
    fi
done
