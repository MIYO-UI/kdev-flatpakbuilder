set(kdevflatpakbuilder_SRCS
    flatpakbuilderplugin.cpp
    flatpakbuilderconfig.cpp
    flatpakmanifestmanager.cpp
    flatpakbuildoutputparser.cpp
    flatpakbuilderjob.cpp
    ui/flatpakbuilderconfigwidget.cpp
)

ki18n_wrap_ui(kdevflatpakbuilder_SRCS
    ui/flatpakbuilderconfigwidget.ui
)

add_subdirectory(icons)