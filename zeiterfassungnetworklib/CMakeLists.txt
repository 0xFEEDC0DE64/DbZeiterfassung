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

add_library(zeiterfassungnetworklib SHARED ${HEADERS} ${SOURCES})

target_compile_definitions(zeiterfassungnetworklib PRIVATE ZEITERFASSUNGNETWORKLIB_LIBRARY)

target_link_libraries(zeiterfassungnetworklib Qt5::Core Qt5::Network dbcorelib)

target_include_directories(zeiterfassungnetworklib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
