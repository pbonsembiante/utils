set(TEST_TARGETS test_plist test_pstack test_pqueue test_pdict test_pexcept)
foreach(TARGET IN LISTS TEST_TARGETS)
  add_executable(${TARGET} ${TARGET}.c)
  target_link_libraries(${TARGET} putils_static unity::framework)
  add_test(${TARGET} ${EXECUTABLE_OUTPUT_PATH}/${TARGET})
endforeach()

# That foreach() should be enough to cover all tests, right? You wish.
# We want to give test_pexcept a special treatment. Why?
#
# Above tests aren't enough to fully test pexcept due to the fact that the library
# allows the user to define a set of hooks at compile time which aren't present
# during the default compilation of the library (this is because, by default,
# we provide the library with all custom flags off)
#
# In order to properly and fully test the library, we should also test the behavior
# of this custom hooks (i.e. that they are executed when they are supposed to)
# we need to recompile pexcept on its own, providing the required definitions
# so that it can pick the correct hook implementations. Note that this is necessary
# due to the fact that the hook resolution happens at compile time of the main library
# thus avoiding whatever custom hooks we add on the tests.

include_directories(include)
add_executable(test_pexcept_hooks ${CMAKE_SOURCE_DIR}/src/pexcept.c test_pexcept_hooks.c)
target_compile_definitions(test_pexcept_hooks PRIVATE PEXCEPT_USE_CONFIG_FILE)
target_link_libraries(test_pexcept_hooks unity::framework)
add_test(test_pexcept_hooks ${EXECUTABLE_OUTPUT_PATH}/test_pexcept_hooks)
