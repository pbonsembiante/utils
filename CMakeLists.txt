cmake_minimum_required(VERSION 3.0.0)
project(PUtils)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Git REQUIRED)

include(ExternalProject)
include(CTest)

if(ENABLE_MEMCHECK)
    find_program( MEMORYCHECK_COMMAND valgrind )
    set(MEMORYCHECK_COMMAND_OPTIONS  "${MEMORYCHECK_COMMAND_OPTIONS} --leak-check=full")
    set(MEMORYCHECK_COMMAND_OPTIONS  "${MEMORYCHECK_COMMAND_OPTIONS} --track-fds=yes")
    set(MEMORYCHECK_COMMAND_OPTIONS  "${MEMORYCHECK_COMMAND_OPTIONS} --trace-children=yes")
    set(MEMORYCHECK_COMMAND_OPTIONS  "${MEMORYCHECK_COMMAND_OPTIONS} --error-exitcode=1")
endif()

if(ENABLE_CPPCHECK)

    list(APPEND CPPCHECK_CMAKE_ARGS
            "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
            )

    ExternalProject_Add(
            cppcheck
            GIT_REPOSITORY      https://github.com/danmar/cppcheck.git
            GIT_TAG             1.81
            GIT_SHALLOW         1
            CMAKE_ARGS          ${CPPCHECK_CMAKE_ARGS}
            PREFIX              ${CMAKE_BINARY_DIR}/external/cppcheck/prefix
            TMP_DIR             ${CMAKE_BINARY_DIR}/external/cppcheck/tmp
            STAMP_DIR           ${CMAKE_BINARY_DIR}/external/cppcheck/stamp
            DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/external/cppcheck/download
            SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/src
            BINARY_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/build
    )

    list(APPEND CPPCHECK_ARGS
            --enable=warning,style,performance,portability
            --std=c11
            --verbose
            --error-exitcode=1
            --language=c
            --suppress=unusedStructMember
            -I ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/include/putils/*.h
            ${CMAKE_SOURCE_DIR}/src/*.c
            ${CMAKE_SOURCE_DIR}/test/*.c
            )

    add_custom_target(
            check
            COMMAND ${CMAKE_BINARY_DIR}/bin/cppcheck ${CPPCHECK_ARGS}
            COMMENT "running cppcheck"
    )

endif()

if(ENABLE_ASTYLE)

    list(APPEND ASTYLE_CMAKE_ARGS
            "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
            )

    ExternalProject_Add(
            astyle
            GIT_REPOSITORY      https://github.com/Bareflank/astyle.git
            GIT_TAG             v1.2
            GIT_SHALLOW         1
            CMAKE_ARGS          ${ASTYLE_CMAKE_ARGS}
            PREFIX              ${CMAKE_BINARY_DIR}/external/astyle/prefix
            TMP_DIR             ${CMAKE_BINARY_DIR}/external/astyle/tmp
            STAMP_DIR           ${CMAKE_BINARY_DIR}/external/astyle/stamp
            DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/external/astyle/download
            SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/astyle/src
            BINARY_DIR          ${CMAKE_BINARY_DIR}/external/astyle/build
    )

    list(APPEND ASTYLE_ARGS
            --options=${CMAKE_SOURCE_DIR}/.astylerc
            ${CMAKE_SOURCE_DIR}/include/putils/*.h
            ${CMAKE_SOURCE_DIR}/src/*.c
            ${CMAKE_SOURCE_DIR}/test/*.c
            )

    if(NOT WIN32 STREQUAL "1")
        add_custom_target(
                format
                COMMAND ${CMAKE_BINARY_DIR}/bin/astyle ${ASTYLE_ARGS}
                COMMENT "running astyle"
        )
    else()
        add_custom_target(
                format
                COMMAND ${CMAKE_BINARY_DIR}/bin/astyle.exe ${ASTYLE_ARGS}
                COMMENT "running astyle"
        )
    endif()

endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(test)
