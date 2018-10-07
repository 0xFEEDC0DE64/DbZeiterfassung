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

add_executable(zeiterfassung ${HEADERS} ${SOURCES} ${FORMS})

target_link_libraries(zeiterfassung Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network zeiterfassungcorelib zeiterfassungguilib zeiterfassungnetworklib)
