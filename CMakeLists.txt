cmake_minimum_required (VERSION 3.0.0)
project(putils)
add_subdirectory(src)
add_subdirectory(test)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message("Current compiler: ${CMAKE_C_COMPILER}")
if(${CMAKE_C_COMPILER} MATCHES ^.+cc.*$ )
    message("Current compiler: ${CMAKE_C_COMPILER}")
    set(CMAKE_C_FLAGS_DEBUG  "${CMAKE_C_FLAGS_DEBUG} --coverage -O0" )
    set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lgcov" )
endif()