add_executable(test_plist test_plist.c)
add_executable(test_pstack test_pstack.c)
add_executable(test_pqueue test_pqueue.c)
add_executable(test_pdict test_pdict.c)

target_link_libraries(test_plist putils_static unity::framework)
target_link_libraries(test_pstack putils_static unity::framework)
target_link_libraries(test_pqueue putils_static unity::framework)
target_link_libraries(test_pdict putils_static unity::framework)

add_test(TEST_PLIST ${EXECUTABLE_OUTPUT_PATH}/test_plist)
add_test(TEST_PSTACK ${EXECUTABLE_OUTPUT_PATH}/test_pstack)
add_test(TEST_PQUEUE ${EXECUTABLE_OUTPUT_PATH}/test_pqueue)
add_test(TEST_PDICT ${EXECUTABLE_OUTPUT_PATH}/test_pdict)
