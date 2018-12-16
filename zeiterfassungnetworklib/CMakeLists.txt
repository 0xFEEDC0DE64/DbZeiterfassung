find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5LinguistTools CONFIG REQUIRED)

set(HEADERS
    zeiterfassungapi.h
    zeiterfassungnetworklib_global.h
    replies/createbookingreply.h
    replies/createtimeassignmentreply.h
    replies/deletebookingreply.h
    replies/deletetimeassignmentreply.h
    replies/getabsencesreply.h
    replies/getbookingsreply.h
    replies/getdayinforeply.h
    replies/getpresencestatusreply.h
    replies/getprojectsreply.h
    replies/getreportreply.h
    replies/gettimeassignmentsreply.h
    replies/getuserinforeply.h
    replies/loginpagereply.h
    replies/loginreply.h
    replies/updatebookingreply.h
    replies/updatetimeassignmentreply.h
    replies/zeiterfassungreply.h
)

set(SOURCES
    zeiterfassungapi.cpp
    replies/createbookingreply.cpp
    replies/createtimeassignmentreply.cpp
    replies/deletebookingreply.cpp
    replies/deletetimeassignmentreply.cpp
    replies/getabsencesreply.cpp
    replies/getbookingsreply.cpp
    replies/getdayinforeply.cpp
    replies/getpresencestatusreply.cpp
    replies/getprojectsreply.cpp
    replies/getreportreply.cpp
    replies/gettimeassignmentsreply.cpp
    replies/getuserinforeply.cpp
    replies/loginpagereply.cpp
    replies/loginreply.cpp
    replies/updatebookingreply.cpp
    replies/updatetimeassignmentreply.cpp
    replies/zeiterfassungreply.cpp
)

set(TRANSLATIONS
    translations/zeiterfassungnetworklib_en.ts
    translations/zeiterfassungnetworklib_de.ts
)

set_source_files_properties(${TRANSLATIONS} PROPERTIES OUTPUT_LOCATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations)
QT5_CREATE_TRANSLATION(TRANSLATIONS_C ${TRANSLATIONS} ${HEADERS} ${SOURCES} OPTIONS -no-obsolete)
#QT5_ADD_TRANSLATION(TRANSLATIONS_C ${TRANSLATIONS})

add_library(zeiterfassungnetworklib SHARED ${HEADERS} ${SOURCES} ${TRANSLATIONS_C})

target_compile_definitions(zeiterfassungnetworklib PRIVATE ZEITERFASSUNGNETWORKLIB_LIBRARY)

target_link_libraries(zeiterfassungnetworklib Qt5::Core Qt5::Network dbcorelib)

target_include_directories(zeiterfassungnetworklib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_dependencies(zeiterfassung zeiterfassungnetworklib)
