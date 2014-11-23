# Copyright (C) 2014 mru@sisyphus.teil.cc


bin=$(readlink -f $TEST_BINARY)
testname=$(basename $0)
workdir=$(readlink -f .)/${testname}_wd

# create working directory, clear automatically on exit
mkdir -p $workdir

trap '{ rm -rf $workdir; }' EXIT
cd $workdir

pwd


