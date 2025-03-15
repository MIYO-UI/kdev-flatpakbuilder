// CMakeLists.txt - główny plik konfiguracyjny projektu
cmake_minimum_required(VERSION 3.16)

project(kdev_flatpakbuilder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(ECM 5.44.0 REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMQtDeclareLoggingCategory)
include(FeatureSummary)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(KF5 REQUIRED COMPONENTS CoreAddons TextEditor I18n ConfigWidgets)
find_package(KDevPlatform REQUIRED)

set(KDEV_FLATPAKBUILDER_SOURCES
    src/flatpakbuilderplugin.cpp
    src/flatpakbuilderconfig.cpp
    src/flatpakmanifestmanager.cpp
    src/flatpakbuildoutputparser.cpp
    src/flatpakbuilderjob.cpp
    src/ui/flatpakbuilderconfigwidget.cpp
)

set(KDEV_FLATPAKBUILDER_HEADERS
    src/flatpakbuilderplugin.h
    src/flatpakbuilderconfig.h
    src/flatpakmanifestmanager.h
    src/flatpakbuildoutputparser.h
    src/flatpakbuilderjob.h
    src/ui/flatpakbuilderconfigwidget.h
)

ki18n_wrap_ui(KDEV_FLATPAKBUILDER_SOURCES
    src/ui/flatpakbuilderconfigwidget.ui
)

add_library(kdevflatpakbuilder MODULE ${KDEV_FLATPAKBUILDER_SOURCES})

target_link_libraries(kdevflatpakbuilder
    KF5::CoreAddons
    KF5::TextEditor
    KF5::I18n
    KF5::ConfigWidgets
    KDev::Interfaces
    KDev::Project
    KDev::OutputView
    KDev::Util
    Qt5::Core
    Qt5::Widgets
)

install(TARGETS kdevflatpakbuilder DESTINATION ${PLUGIN_INSTALL_DIR}/kdevplatform/${KDEV_PLUGIN_VERSION})
install(FILES kdevflatpakbuilder.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES kdevflatpakbuilder.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/kdevflatpakbuilder)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)