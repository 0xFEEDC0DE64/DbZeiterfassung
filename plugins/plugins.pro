TEMPLATE = subdirs

SUBDIRS += absenceplugin \
           advancedviewplugin \
           devtoolsplugin \
           lunchmealplugin \
           profileplugin \
           presenceplugin \
           reportsplugin \
           sketchplugin \
           updaterplugin \
           weatherplugin
qtHaveModule(multimedia): SUBDIRS += webradioplugin
else: message("multimedia is missing, will not build webradioplugin")

OTHER_FILES += plugin.pri
