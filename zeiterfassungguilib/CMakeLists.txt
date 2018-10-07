set(HEADERS
    mainwindow.h
    settingswidget.h
    stripfactory.h
    stripswidget.h
    zeiterfassungdialog.h
    zeiterfassungguilib_global.h
    zeiterfassungplugin.h
    dialogs/authenticationdialog.h
    dialogs/languageselectiondialog.h
    dialogs/settingsdialog.h
)

set(SOURCES
    mainwindow.cpp
    settingswidget.cpp
    stripfactory.cpp
    stripswidget.cpp
    zeiterfassungdialog.cpp
    zeiterfassungplugin.cpp
    dialogs/authenticationdialog.cpp
    dialogs/languageselectiondialog.cpp
    dialogs/settingsdialog.cpp
)

set(FORMS
    mainwindow.ui
    dialogs/authenticationdialog.ui
    dialogs/languageselectiondialog.ui
    dialogs/settingsdialog.ui
)

add_library(zeiterfassungguilib SHARED ${HEADERS} ${SOURCES} ${FORMS})

target_compile_definitions(zeiterfassungguilib PRIVATE ZEITERFASSUNGGUILIB_LIBRARY)

target_link_libraries(zeiterfassungguilib Qt5::Core Qt5::Gui Qt5::Widgets Qt5::UiTools zeiterfassungcorelib zeiterfassungnetworklib)

target_include_directories(zeiterfassungguilib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
