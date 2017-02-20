AC_PREREQ([2.69])
AC_INIT([putils], [0.1.1], [pbonsembiante@gmail.com])
AC_CONFIG_SRCDIR([include/collections/pnode.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# m4 separate folder
AC_CONFIG_MACRO_DIRS([m4])

# Add call to automake
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall -Werror])
AM_PROG_AR

# Call libtools to build a library
LT_INIT

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strdup])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
