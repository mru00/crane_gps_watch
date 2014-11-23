# Copyright (C) 2014 mru@sisyphus.teil.cc

AC_INIT([crane_gps_watch_client], [0.2], [mru@sisyphus.teil.cc], [crane_gps_watch_client], [https://github.com/mru00/crane_gps_watch])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
                 Makefile
                 src/Makefile
                 test/Makefile
                 data/Makefile
                 ])

# requires automake 1.13 [https://autotools.io/autoconf/macros.html] -- breaks travis-ci.org
#AC_CONFIG_MACRO_DIR([m4])
# this works with older versions:
m4_include([m4/ax_cxx_compile_stdcxx_11.m4])

AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11
AC_PROG_CXX
AC_PROG_CC

AC_CANONICAL_HOST

AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([termios.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_FUNCS([strerror])
AC_CHECK_HEADER_STDBOOL
AC_FUNC_MALLOC
AC_FUNC_MKTIME

AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

AM_CONDITIONAL([CROSS_COMPILING], test "$cross_compiling" = yes)

AC_OUTPUT
