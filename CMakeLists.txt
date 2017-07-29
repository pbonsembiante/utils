cmake_minimum_required (VERSION 3.0.0)
project(putils)
add_subdirectory(src)
add_subdirectory(test)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
