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

install(TARGETS putils_static putils_shared DESTINATION lib)
install(FILES ${PUTILS_HEADERS} DESTINATION include)
