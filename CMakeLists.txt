cmake_minimum_required(VERSION 2.8.9)
project(QtYARP CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
include(FeatureSummary)
include(CMakeDependentOption)

# TODO Find out why these are needed
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# Postfix for debug builds
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui Quick Qml Multimedia Xml PrintSupport)
set_package_properties(Qt5 PROPERTIES TYPE REQUIRED)
set_package_properties(Qt5Core PROPERTIES TYPE REQUIRED)
set_package_properties(Qt5Widgets PROPERTIES TYPE REQUIRED)
set_package_properties(Qt5Gui PROPERTIES TYPE REQUIRED)
set_package_properties(Qt5Quick PROPERTIES TYPE REQUIRED)
set_package_properties(Qt5Qml PROPERTIES TYPE REQUIRED)
set_package_properties(Qt5Multimedia PROPERTIES TYPE REQUIRED)
set_package_properties(Qt5Xml PROPERTIES TYPE REQUIRED)
set_package_properties(Qt5PrintSupport PROPERTIES TYPE REQUIRED)
set_package_properties(Qt5Network PROPERTIES TYPE REQUIRED)

if(Qt5_FOUND)
    get_property(_packages_found GLOBAL PROPERTY PACKAGES_FOUND)
    get_property(_packages_not_found GLOBAL PROPERTY PACKAGES_NOT_FOUND)
    list(REMOVE_ITEM _packages_found Qt5Core Qt5Widgets Qt5Gui Qt5Quick Qt5Qml Qt5Multimedia Qt5Xml Qt5PrintSupport Qt5Network)
    list(REMOVE_ITEM _packages_not_found Qt5Core Qt5Widgets Qt5Gui Qt5Quick Qt5Qml Qt5Multimedia Qt5Xml Qt5PrintSupport Qt5Network)
    set_property(GLOBAL PROPERTY PACKAGES_FOUND ${_packages_found})
    set_property(GLOBAL PROPERTY PACKAGES_NOT_FOUND ${_packages_not_found})
endif()

find_package(YARP 2.3.62.1)
set_package_properties(YARP PROPERTIES TYPE REQUIRED)

include_directories(${YARP_INCLUDE_DIRS})


if(WIN32)
  set(PLUGINS_RELATIVE_PATH "..\\${CMAKE_INSTALL_LIBDIR}\\qml\\imports")
else()
  set(PLUGINS_RELATIVE_PATH "../${CMAKE_INSTALL_LIBDIR}/qml/imports")
endif()
configure_file(config.h.in ${CMAKE_BINARY_DIR}/config.h @ONLY)
include_directories(${CMAKE_BINARY_DIR})


option(WITH_QTYARPVIEW "Enable QtYARPViewPlugin and QtYARPView" ON)
add_feature_info(QtYARPView WITH_QTYARPVIEW "QtYARPViewPlugin and QtYARPView.")
if(WITH_QTYARPVIEW)
    add_subdirectory(QtYARPViewPlugin)
    add_subdirectory(QtYARPView)
endif()

option(WITH_QTYARPSCOPE "Enable QtYARPScopePlugin and QtYARPScope" ON)
add_feature_info(QtYARPScope WITH_QTYARPSCOPE "QtYARPScopePlugin and QtYARPScope.")
if(WITH_QTYARPSCOPE)
    add_subdirectory(QtYARPScopePlugin)
    add_subdirectory(QtYARPScope)
endif()

option(WITH_QTYARPMANAGER "Enable QtYARPManager" ${YARP_HAS_MANAGER_LIB})
if(WITH_QTYARPMANAGER AND NOT YARP_HAS_MANAGER_LIB)
    message(WARNING "
YARP_EXPERIMENTAL_MANAGER_LIB flag must be enabled in YARP in order to build QtYARPManager.
The option \"WITH_QTYARPMANAGER\" will now be disabled.")
    set_property(CACHE WITH_QTYARPMANAGER PROPERTY VALUE OFF)
endif()
add_feature_info(QtYARPManager WITH_QTYARPMANAGER "QtYARPManager.")
if(WITH_QTYARPMANAGER)
    add_subdirectory(QtYARPManager)
endif()

cmake_dependent_option(WITH_EXAMPLES "Enable Examples" OFF
                                     "WITH_QTYARPVIEW;WITH_QTYARPSCOPE" OFF)
add_feature_info(Examples WITH_EXAMPLES "Examples.")
if(WITH_EXAMPLES)
    add_subdirectory(ContainerExample)
endif()



feature_summary(WHAT PACKAGES_FOUND
                     REQUIRED_PACKAGES_NOT_FOUND
                     RECOMMENDED_PACKAGES_NOT_FOUND
                     FATAL_ON_MISSING_REQUIRED_PACKAGES)

feature_summary(WHAT ENABLED_FEATURES
                     DISABLED_FEATURES)

include(AddUninstallTarget)
