# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.12])
AC_INIT([amm], [4.0.0], [https://github.com/chiku/amm/issues])
AC_LANG(C++)

# Silent by default
AM_SILENT_RULES([yes])

AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([parallel-tests color-tests])

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL

# Checks for header files.
AC_CHECK_HEADERS([dirent.h getopt.h])

# Checks for library functions.
AC_CHECK_FUNCS([amm])
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
