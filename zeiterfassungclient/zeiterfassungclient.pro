QT += core network gui widgets

DBLIBS += zeiterfassungcore zeiterfassunggui zeiterfassungnetwork

TARGET = zeiterfassung

PROJECT_ROOT = ../..

RC_ICONS = icon.ico

SOURCES += main.cpp

HEADERS +=

FORMS += strips/bookingstartstrip.ui \
         strips/bookingendstrip.ui \
         strips/timeassignmentstrip.ui

RESOURCES += zeiterfassung_resources.qrc

TRANSLATIONS += translations/zeiterfassung_en.ts \
                translations/zeiterfassung_de.ts

include($${PROJECT_ROOT}/app.pri)
include(installs.pri)
