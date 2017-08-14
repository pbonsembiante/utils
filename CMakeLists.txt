cmake_minimum_required (VERSION 3.0.0)
project(PUtils)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(test)
