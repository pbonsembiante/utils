ACLOCAL_AMFLAGS = -I m4

INCLUDE_DIR = ./include/
UNITY_DIR = ./vendor/unity/src 
TEST_INC_DIR = -I$(INCLUDE_DIR) -I$(UNITY_DIR)

TESTS = $(check_PROGRAMS)

check_PROGRAMS = test_plist test_pqueue test_pstack

test_pstack_SOURCES = \
	src/pstack.c				\
	src/plist.c					\
	test/test_pstack.c			\
	vendor/unity/src/unity.c

test_pqueue_SOURCES = \
	src/pqueue.c				\
	src/plist.c					\
	test/test_pqueue.c			\
	vendor/unity/src/unity.c	

test_plist_SOURCES = \
	src/plist.c					\
	test/test_plist.c 			\
	vendor/unity/src/unity.c			

test_plist_CFLAGS = $(TEST_INC_DIR)
test_pqueue_CFLAGS = $(TEST_INC_DIR)
test_pstack_CFLAGS = $(TEST_INC_DIR)

lib_LTLIBRARIES = libputils.la

libputils_la_SOURCES = \
	src/plist.c					\
	src/pdict.c					\
	src/pexcept.c				\
	src/pqueue.c				\
	src/pstack.c


pkginclude_HEADERS = \
	include/putils/plist.h		\
	include/putils/pdict.h		\
	include/putils/pnode.h		\
	include/putils/pexcept.h	\
	include/putils/pstack.h		\
	include/putils/pqueue.h


libputils_la_CFLAGS = -I$(INCLUDE_DIR)

dist_doc_DATA = \
	README.md

EXTRA_DIST = m4/NOTES

