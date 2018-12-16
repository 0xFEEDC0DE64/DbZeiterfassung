find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5UiTools CONFIG REQUIRED)
find_package(Qt5LinguistTools CONFIG REQUIRED)

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

set(RESOURCES
    zeiterfassungguilib_resources.qrc
)

set(TRANSLATIONS
    translations/zeiterfassungguilib_en.ts
    translations/zeiterfassungguilib_de.ts
)

set_source_files_properties(${TRANSLATIONS} PROPERTIES OUTPUT_LOCATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations)
QT5_CREATE_TRANSLATION(TRANSLATIONS_C ${TRANSLATIONS} ${HEADERS} ${SOURCES} ${FORMS} OPTIONS -no-obsolete)
#QT5_ADD_TRANSLATION(TRANSLATIONS_C ${TRANSLATIONS})

add_library(zeiterfassungguilib SHARED ${HEADERS} ${SOURCES} ${FORMS} ${RESOURCES} ${TRANSLATIONS_C})

target_compile_definitions(zeiterfassungguilib PRIVATE ZEITERFASSUNGGUILIB_LIBRARY)

target_link_libraries(zeiterfassungguilib Qt5::Core Qt5::Gui Qt5::Widgets Qt5::UiTools zeiterfassungcorelib zeiterfassungnetworklib)

target_include_directories(zeiterfassungguilib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_dependencies(zeiterfassung zeiterfassungguilib)
