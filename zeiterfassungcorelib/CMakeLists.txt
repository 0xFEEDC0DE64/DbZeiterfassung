set(HEADERS
    zeiterfassungcorelib_global.h
    zeiterfassungsettings.h
)

set(SOURCES
    zeiterfassungsettings.cpp
)

add_library(zeiterfassungcorelib SHARED ${HEADERS} ${SOURCES})

target_compile_definitions(zeiterfassungcorelib PRIVATE ZEITERFASSUNGCORELIB_LIBRARY)

target_link_libraries(zeiterfassungcorelib Qt5::Core dbcorelib)

target_include_directories(zeiterfassungcorelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_dependencies(zeiterfassung zeiterfassungcorelib)
