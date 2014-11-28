# Copyright (C) 2014 mru@sisyphus.teil.cc


bin=$(readlink -f $TEST_BINARY)
testname=$(basename $0)
workdir=$(readlink -f .)/${testname}_wd
srcdir_abs=$(readlink -f $srcdir)

# create working directory, clear automatically on exit
mkdir -p $workdir

trap '{ rm -rf $workdir; }' EXIT
cd $workdir

pwd


expect_exit() {
  local expected=$1
  shift

  set +e

  #valgrind --error-exitcode=3  "$@"
  "$@"

  code=$?;

  if [[ $code -ne $expected ]]; then
    echo "testcase did not exit with expected code; actual=$code, expected=$expected"
    exit 1
  fi

}

