include_directories(${CMAKE_SOURCE_DIR}/include)

set(PUTILS_SOURCES
        ${CMAKE_SOURCE_DIR}/include/putils/pdict.h
        ${CMAKE_SOURCE_DIR}/include/putils/pexcept.h
        ${CMAKE_SOURCE_DIR}/include/putils/plist.h
        ${CMAKE_SOURCE_DIR}/include/putils/pnode.h
        ${CMAKE_SOURCE_DIR}/include/putils/pqueue.h
        ${CMAKE_SOURCE_DIR}/include/putils/pstack.h
        pdict.c
        pexcept.c
        plist.c
        pqueue.c
        pstack.c)

set(PUTILS_HEADERS
        ${CMAKE_SOURCE_DIR}/include/putils/pdict.h
        ${CMAKE_SOURCE_DIR}/include/putils/pexcept.h
        ${CMAKE_SOURCE_DIR}/include/putils/plist.h
        ${CMAKE_SOURCE_DIR}/include/putils/pnode.h
        ${CMAKE_SOURCE_DIR}/include/putils/pqueue.h
        ${CMAKE_SOURCE_DIR}/include/putils/pstack.h)

add_library(putilsobj OBJECT ${PUTILS_SOURCES})
add_library(putils_shared SHARED $<TARGET_OBJECTS:putilsobj>)
add_library(putils_static STATIC $<TARGET_OBJECTS:putilsobj>)

install(TARGETS putils_static putils_shared DESTINATION lib )
install(FILES ${PUTILS_HEADERS} DESTINATION include)