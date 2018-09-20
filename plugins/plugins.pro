TEMPLATE = subdirs

exists(absenceplugin/absenceplugin.pro): SUBDIRS += absenceplugin
exists(advancedviewplugin/advancedviewplugin.pro): SUBDIRS += advancedviewplugin
exists(devtoolsplugin/devtoolsplugin.pro): SUBDIRS += devtoolsplugin
exists(lunchmealplugin/lunchmealplugin.pro): SUBDIRS += lunchmealplugin
exists(profileplugin/profileplugin.pro): SUBDIRS += profileplugin
exists(presenceplugin/presenceplugin.pro): SUBDIRS += presenceplugin
exists(reportsplugin/reportsplugin.pro): SUBDIRS += reportsplugin
exists(sketchplugin/sketchplugin.pro): SUBDIRS += sketchplugin
exists(updaterplugin/updaterplugin.pro): SUBDIRS += updaterplugin
exists(weatherplugin/weatherplugin.pro): SUBDIRS += weatherplugin
exists(webradioplugin/webradioplugin.pro): {
    qtHaveModule(multimedia): SUBDIRS += webradioplugin
    else: message("multimedia is missing, will not build webradioplugin")
}

OTHER_FILES += plugin.pri
