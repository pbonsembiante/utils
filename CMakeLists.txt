cmake_minimum_required (VERSION 3.0.0)
project(PUtils)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(PUTILS_SOURCES
        include/putils/pdict.h
        include/putils/pexcept.h
        include/putils/plist.h
        include/putils/pnode.h
        include/putils/pqueue.h
        include/putils/pstack.h
        src/pdict.c
        src/pexcept.c
        src/plist.c
        src/pqueue.c
        src/pstack.c)

set(PUTILS_HEADERS
        include/putils/pdict.h
        include/putils/pexcept.h
        include/putils/plist.h
        include/putils/pnode.h
        include/putils/pqueue.h
        include/putils/pstack.h)

set(UNITY_SOURCES
        vendor/unity/src/unity.c
        vendor/unity/src/unity.h
        vendor/unity/src/unity_internals.h)

include_directories(include)
include_directories(include/putils)
include_directories(vendor/unity/src)

add_library(putilsobj OBJECT ${PUTILS_SOURCES})
add_library(putils_shared SHARED $<TARGET_OBJECTS:putilsobj>)
add_library(putils_static STATIC $<TARGET_OBJECTS:putilsobj>)
add_library(unitylib STATIC ${UNITY_SOURCES})

add_executable(test_plist
        test/test_plist.c)

target_link_libraries(test_plist
        putils_shared
        unitylib)

add_executable(test_pstack
        test/test_pstack.c)

target_link_libraries(test_pstack
        putils_shared
        unitylib)

add_executable(test_pqueue
        test/test_pqueue.c)

target_link_libraries(test_pqueue
        putils_shared
        unitylib)

enable_testing()
add_test(TEST_PLIST test_plist)
add_test(TEST_PLIST test_pstack)
add_test(TEST_PLIST test_pqueue)

install(TARGETS putils_static putils_shared DESTINATION lib )
install(FILES ${PUTILS_HEADERS} DESTINATION include)
