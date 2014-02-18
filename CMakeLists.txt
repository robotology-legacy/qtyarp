cmake_minimum_required(VERSION 2.8.11)
project(QtYARP)

include(GNUInstallDirs)

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_DEBUG_POSTFIX "d")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Quick)
find_package(Qt5Qml)
find_package(Qt5Multimedia)
find_package(Qt5Xml)
find_package(Qt5PrintSupport)

find_package(YARP)
find_package(YARPManager)
include_directories(${YARP_INCLUDE_DIRS})

add_subdirectory(QtYarpManager)
add_subdirectory(QtYARPScope)
add_subdirectory(QtYARPScopePlugin)
add_subdirectory(QtYARPView)
add_subdirectory(QtYARPViewPlugin)
add_subdirectory(ContainerExample)
