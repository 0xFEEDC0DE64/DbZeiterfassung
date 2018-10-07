QT += core
QT -= gui widgets

DBLIBS += dbcore

TARGET = zeiterfassungcore

PROJECT_ROOT = ../..

DEFINES += ZEITERFASSUNGCORELIB_LIBRARY

SOURCES += \
           zeiterfassungsettings.cpp

HEADERS += zeiterfassungcorelib_global.h \
           zeiterfassungsettings.h

FORMS +=

RESOURCES +=

TRANSLATIONS += translations/zeiterfassungcorelib_en.ts \
                translations/zeiterfassungcorelib_de.ts

include($${PROJECT_ROOT}/lib.pri)
