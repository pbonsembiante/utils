set(PUTILS_HEADERS
    ${CMAKE_SOURCE_DIR}/include/putils/pdict.h
    ${CMAKE_SOURCE_DIR}/include/putils/pexcept.h
    ${CMAKE_SOURCE_DIR}/include/putils/plist.h
    ${CMAKE_SOURCE_DIR}/include/putils/pnode.h
    ${CMAKE_SOURCE_DIR}/include/putils/pqueue.h
    ${CMAKE_SOURCE_DIR}/include/putils/pstack.h)

set(PUTILS_SOURCES
    ${PUTILS_HEADERS}
    pdict.c
    pexcept.c
    plist.c
    pqueue.c
    pstack.c)

add_library(putilsobj OBJECT ${PUTILS_SOURCES})
add_library(putils_shared SHARED $<TARGET_OBJECTS:putilsobj>)
add_library(putils_static STATIC $<TARGET_OBJECTS:putilsobj>)

set_target_properties(putils_shared
    PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    PUBLIC_HEADER "${PUTILS_HEADERS}"
    EXPORT_NAME framework
    )

set_target_properties(putils_static
    PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    PUBLIC_HEADER "${PUTILS_HEADERS}"
    EXPORT_NAME framework
    )

target_compile_options(putilsobj
    PRIVATE
    $<$<C_COMPILER_ID:Clang>:
      -Wcast-align
      -Wcast-qual
      -Wconversion
      -Wexit-time-destructors
      -Wglobal-constructors
      -Wmissing-noreturn
      -Wmissing-prototypes
      -Wno-missing-braces
      -Wold-style-cast
      -Wshadow
      -Wweak-vtables
      -Werror
      -Wall>
    $<$<C_COMPILER_ID:GNU>:
      -Waddress
      -Wformat-nonliteral
      -Wformat-security
      -Wformat
      -Winit-self
      -Wmissing-declarations
      -Wmissing-include-dirs
      -Wno-multichar
      -Wno-parentheses
      -Wno-type-limits
      -Wno-unused-parameter
      -Wunreachable-code
      -Wwrite-strings
      -Wpointer-arith
      -Werror
      -Wall>
    $<$<C_COMPILER_ID:MSVC>:/Wall>
    )

## Target's cmake files: targets export
install(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

## Target installation
install(TARGETS putils_static putils_shared
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    COMPONENT library
    )

## Target's cmake files: config and version config for find_package()
install(FILES ../${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/../${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
