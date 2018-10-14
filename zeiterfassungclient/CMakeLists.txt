set(HEADERS
)

set(SOURCES
    main.cpp
)

set(RESOURCES
    zeiterfassung_resources.qrc
)

set(FORMS
    strips/bookingendstrip.ui
    strips/bookingstartstrip.ui
    strips/timeassignmentstrip.ui
)

# TODO: add icon.ico for windows
add_executable(zeiterfassungclient ${HEADERS} ${SOURCES} ${RESOURCES} ${FORMS})

target_link_libraries(zeiterfassungclient Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network zeiterfassungcorelib zeiterfassungguilib zeiterfassungnetworklib)

add_dependencies(zeiterfassung zeiterfassungclient)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/themes ${CMAKE_CURRENT_SOURCE_DIR}/strips
     DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
