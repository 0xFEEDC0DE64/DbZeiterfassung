QT += core network
QT -= gui widgets

DBLIBS += dbcore

TARGET = zeiterfassungnetwork

PROJECT_ROOT = ../..

DEFINES += ZEITERFASSUNGNETWORKLIB_LIBRARY

SOURCES += \
           zeiterfassungapi.cpp \
           replies/createbookingreply.cpp \
           replies/createtimeassignmentreply.cpp \
           replies/deletebookingreply.cpp \
           replies/deletetimeassignmentreply.cpp \
           replies/getabsencesreply.cpp \
           replies/getbookingsreply.cpp \
           replies/getdayinforeply.cpp \
           replies/getpresencestatusreply.cpp \
           replies/getprojectsreply.cpp \
           replies/getreportreply.cpp \
           replies/gettimeassignmentsreply.cpp \
           replies/getuserinforeply.cpp \
           replies/loginpagereply.cpp \
           replies/loginreply.cpp \
           replies/updatebookingreply.cpp \
           replies/updatetimeassignmentreply.cpp \
           replies/zeiterfassungreply.cpp

HEADERS += zeiterfassungnetworklib_global.h \
           zeiterfassungapi.h \
           replies/createbookingreply.h \
           replies/createtimeassignmentreply.h \
           replies/deletebookingreply.h \
           replies/deletetimeassignmentreply.h \
           replies/getabsencesreply.h \
           replies/getbookingsreply.h \
           replies/getdayinforeply.h \
           replies/getpresencestatusreply.h \
           replies/getprojectsreply.h \
           replies/getreportreply.h \
           replies/gettimeassignmentsreply.h \
           replies/getuserinforeply.h \
           replies/loginpagereply.h \
           replies/loginreply.h \
           replies/updatebookingreply.h \
           replies/updatetimeassignmentreply.h \
           replies/zeiterfassungreply.h

FORMS +=

RESOURCES +=

TRANSLATIONS += translations/zeiterfassungnetworklib_en.ts \
                translations/zeiterfassungnetworklib_de.ts

include($${PROJECT_ROOT}/lib.pri)
