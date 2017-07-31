include_directories(${CMAKE_SOURCE_DIR}/include)

set(UNITY_SOURCES
        ${CMAKE_SOURCE_DIR}/vendor/unity/src/unity.c
        ${CMAKE_SOURCE_DIR}/vendor/unity/src/unity.h
        ${CMAKE_SOURCE_DIR}/vendor/unity/src/unity_internals.h)

include_directories(${CMAKE_SOURCE_DIR}/vendor/unity/src)

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
