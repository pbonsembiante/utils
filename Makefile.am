ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libputils.la

libputils_la_SOURCES = \
	src/collections/plist.c \
	src/collections/pdict.c \
	include/collections/plist.h \
	include/collections/pdict.h \
	include/collections/pnode.h

libputils_la_CFLAGS = -Iinclude/collections/ -Iinclude/ -std=c11 -Wall -Wextra

dist_doc_DATA = README.md

EXTRA_DIST = m4/NOTES

