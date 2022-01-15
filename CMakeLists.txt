cmake_minimum_required(VERSION 3.14.0)

project(
    putils
    VERSION 0.0.1
    LANGUAGES C
    DESCRIPTION "A set of utility libraries"
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ENABLE_EXPORTS ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${PROJECT_SOURCE_DIR}/cmake)

find_package(Git REQUIRED)

include(CTest)
include(FetchContent)
include(ExternalProject)
include(FetchExternalContent)
include(CppCheck)
include(MemCheck)
include(AStyle)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(test)

write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
