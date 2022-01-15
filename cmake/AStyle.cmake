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
