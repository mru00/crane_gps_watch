AC_INIT([crane_gps_watch_client], [0.2-dev], [mru@sisyphus.teil.cc])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
                 Makefile
                 src/Makefile
                 test/Makefile
                 ])
AC_CONFIG_MACRO_DIR([m4])

PKG_CHECK_MODULES(XML, libxml-2.0 >= 2.4)

AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11
AC_PROG_CXX


AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([termios.h])
AC_CHECK_HEADERS([unistd.h])

AM_MISSING_PROG(HELP2MAN, help2man, $missing_dir)

# doesn't work on travis-ci.org...
#AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

AC_OUTPUT
