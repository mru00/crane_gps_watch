# vim: ft=sh
# Copyright (C) 2014 mru@sisyphus.teil.cc


# see http://www.gnu.org/software/automake/manual/html_node/Scripts_002dbased-Testsuites.html

bin=$(readlink -f $TEST_BINARY)
testname=$(basename $0)
workdir=$(readlink -f .)/${testname}_wd
srcdir_abs=$(readlink -f $srcdir)

readonly EXIT_SKIP=77
readonly EXIT_SUCCESS=0
readonly EXIT_FAIL=1

# create working directory, clear automatically on exit
[[ -d $workdir ]] && rm -rf $workdir
mkdir -p $workdir

#trap '{ rm -rf $workdir; }' EXIT
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
    exit EXIT_FAIL
  fi

}

get_xml_tag_count() {
    local tcx_file=$1
    local tag=$2
    grep -c "<$tag" $tcx_file || true
}

expect_lap_count() {
    local tcx_file=$1
    local count=$2
    expect_exit 0 test $(get_xml_tag_count $tcx_file Lap) -eq $count
}

