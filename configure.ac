AC_INIT([crane_gps_watch_client], [0.2], [mru@sisyphus.teil.cc])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
                 Makefile
                 src/Makefile
                 test/Makefile
                 ])

# requires automake 1.13 [https://autotools.io/autoconf/macros.html] -- breaks travis-ci.org
#AC_CONFIG_MACRO_DIR([m4])
# this works with older versions:
m4_include([m4/ax_cxx_compile_stdcxx_11.m4])

PKG_CHECK_MODULES(XML, libxml-2.0 >= 2.4)

AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11
AC_PROG_CXX


AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([termios.h])
AC_CHECK_HEADERS([unistd.h])

# doesn't work on travis-ci.org...
#AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

AC_OUTPUT
