set(UNITY_DIR
        ${CMAKE_SOURCE_DIR}/vendor/unity/src)

include_directories(${UNITY_DIR})

set(UNITY_SOURCES
        ${UNITY_DIR}/unity.c
        ${UNITY_DIR}/unity.h
        ${UNITY_DIR}/unity_internals.h)

add_library(unitylib STATIC ${UNITY_SOURCES})

add_executable(test_plist test_plist.c)
add_executable(test_pstack test_pstack.c)
add_executable(test_pqueue test_pqueue.c)

target_link_libraries(test_plist putils_static unitylib)
target_link_libraries(test_pstack putils_static unitylib)
target_link_libraries(test_pqueue putils_static unitylib)

enable_testing()
add_test(TEST_PLIST test_plist)
add_test(TEST_PSTACK test_pstack)
add_test(TEST_PQUEUE test_pqueue)

message("Flags = " ${CMAKE_C_FLAGS_DEBUG})