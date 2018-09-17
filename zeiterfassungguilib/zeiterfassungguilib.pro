QT += core network gui widgets uitools

DBLIBS += dbcore zeiterfassungcore

TARGET = zeiterfassunggui

PROJECT_ROOT = ../..

DEFINES += ZEITERFASSUNGGUILIB_LIBRARY

SOURCES += mainwindow.cpp \
           settingswidget.cpp \
           stripfactory.cpp \
           stripswidget.cpp \
           zeiterfassungdialog.cpp \
           zeiterfassungplugin.cpp \
           dialogs/authenticationdialog.cpp \
           dialogs/languageselectiondialog.cpp \
           dialogs/settingsdialog.cpp

HEADERS += mainwindow.h \
           settingswidget.h \
           stripfactory.h \
           stripswidget.h \
           zeiterfassungguilib_global.h \
           zeiterfassungdialog.h \
           zeiterfassungplugin.h \
           dialogs/authenticationdialog.h \
           dialogs/languageselectiondialog.h \
           dialogs/settingsdialog.h

FORMS += mainwindow.ui \
         dialogs/settingsdialog.ui \
         dialogs/languageselectiondialog.ui \
         dialogs/authenticationdialog.ui

RESOURCES += zeiterfassungguilib_resources.qrc

TRANSLATIONS += translations/zeiterfassungguilib_en.ts \
                translations/zeiterfassungguilib_de.ts

include($${PROJECT_ROOT}/lib.pri)
