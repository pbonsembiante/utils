ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libputils.la

libputils_la_SOURCES = \
	src/plist.c					\
	src/pdict.c					\
	src/pexcept.c				\
	include/putils/plist.h		\
	include/putils/pdict.h		\
	include/putils/pnode.h		\
	include/putils/pexcept.h


pkginclude_HEADERS = \
	include/putils/plist.h		\
	include/putils/pdict.h		\
	include/putils/pnode.h		\
	include/putils/pexcept.h


libputils_la_CFLAGS = -Iinclude/ -std=c11 -Wall -Wextra

dist_doc_DATA = README.md

EXTRA_DIST = m4/NOTES

