#! /bin/bash -eux

# Copyright (C) 2015 mru@sisyphus.teil.cc

source ${srcdir}/common.sh

first_letter_uppercase() {
    echo $1 | perl -p -e 's/^(.)/\U$1/'
}

while read filename profile tcx_profile; do
    ln -s ${srcdir_abs}/profile_${filename}.bin
    expect_exit 0 $bin --from_image profile_${filename}.bin --verbose --output ${filename}.tcx | tee profile.log
    expect_exit 0 grep "workout info p=${profile}" profile.log
    expect_exit 0 grep "Sport=\"${tcx_profile}\"" ${filename}.tcx
done <<EOF
german_laufen Laufen Running
german_radfahren Radfahren Biking
german_wandern Wandern Other
english_running Running Running
english_cycling Cycling Biking
EOF

