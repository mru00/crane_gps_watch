#! /bin/bash -eu

source ${srcdir}/common.sh


while read lc uc; do
    ln -s ${srcdir_abs}/knedlyk_${lc}.bin
    expect_exit 0 $bin --from_image knedlyk_${lc}.bin --verbose | tee ${lc}.log
    expect_exit 0 grep "workout info p=${uc}" ${lc}.log
done <<EOF
running Running
cycling Cycling
EOF
