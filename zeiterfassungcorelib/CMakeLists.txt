find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5LinguistTools CONFIG REQUIRED)

set(HEADERS
    zeiterfassungcorelib_global.h
    zeiterfassungsettings.h
)

set(SOURCES
    zeiterfassungsettings.cpp
)

set(TRANSLATIONS
    translations/zeiterfassungcorelib_en.ts
    translations/zeiterfassungcorelib_de.ts
)

set_source_files_properties(${TRANSLATIONS} PROPERTIES OUTPUT_LOCATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations)
QT5_CREATE_TRANSLATION(TRANSLATIONS_C ${TRANSLATIONS} ${HEADERS} ${SOURCES} OPTIONS -no-obsolete)
#QT5_ADD_TRANSLATION(TRANSLATIONS_C ${TRANSLATIONS})

add_library(zeiterfassungcorelib SHARED ${HEADERS} ${SOURCES} ${TRANSLATIONS_C})

target_compile_definitions(zeiterfassungcorelib PRIVATE ZEITERFASSUNGCORELIB_LIBRARY)

target_link_libraries(zeiterfassungcorelib Qt5::Core dbcorelib)

target_include_directories(zeiterfassungcorelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_dependencies(zeiterfassung zeiterfassungcorelib)
