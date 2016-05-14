#! /bin/bash -eux

# Copyright (C) 2015 mru@sisyphus.teil.cc

source ${srcdir}/common.sh

for img in ${srcdir_abs}/lang*.bin; do
    ln -s $img
done

while read filename language; do 

    expect_exit 0 $bin --from_image $filename --verbose > logfile
    expect_exit 0 grep "l=$language" logfile

done <<EOF
lang_german.bin German
lang_english.bin English
lang_italian.bin Italian
lang_french.bin French
lang_spanish.bin Spanish
EOF

