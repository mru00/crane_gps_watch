#! /bin/bash -eux

# Copyright (C) 2015 mru@sisyphus.teil.cc

source ${srcdir}/common.sh


readonly faketime=/usr/lib/x86_64-linux-gnu/faketime/libfaketime.so.1

if ! [[ -e $faketime ]]; then
    echo "libfaketime not found; can't execute testcases"
    exit 77
fi


export LD_PRELOAD=$faketime
export FAKETIME="2014-01-01 12:00:00"
$bin --help

# WIP
exit 77

