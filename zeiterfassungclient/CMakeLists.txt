find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5LinguistTools CONFIG REQUIRED)

set(HEADERS
)

set(SOURCES
    main.cpp
)

set(FORMS
    strips/bookingendstrip.ui
    strips/bookingstartstrip.ui
    strips/timeassignmentstrip.ui
)

set(RESOURCES
    zeiterfassung_resources.qrc
)

set(TRANSLATIONS
    translations/zeiterfassungclient_en.ts
    translations/zeiterfassungclient_de.ts
)

set_source_files_properties(${TRANSLATIONS} PROPERTIES OUTPUT_LOCATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations)
QT5_CREATE_TRANSLATION(TRANSLATIONS_C ${TRANSLATIONS} ${HEADERS} ${SOURCES} ${FORMS} OPTIONS -no-obsolete)
#QT5_ADD_TRANSLATION(TRANSLATIONS_C ${TRANSLATIONS})

# TODO: add icon.ico for windows
add_executable(zeiterfassungclient ${HEADERS} ${SOURCES} ${FORMS} ${RESOURCES} ${TRANSLATIONS_C})

target_link_libraries(zeiterfassungclient Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network zeiterfassungcorelib zeiterfassungguilib zeiterfassungnetworklib)

add_dependencies(zeiterfassung zeiterfassungclient)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/themes ${CMAKE_CURRENT_SOURCE_DIR}/strips
     DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
