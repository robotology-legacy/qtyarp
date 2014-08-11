QtYARP
======

QtYARP is a porting of YARP Gtk/GtkMM GUIs to Qt5.

 - QtYARPView (porting of yarpview)
 - QtYARPScope (porting of yarpscope)
 - QtYARPManager (porting of gyarpmanager)

Also includes 2 QML Plugins that can be included by any QtQuick
application:

 - QtYARPViewPlugin
 - QtYARPScopePlugin

The ContainerExample application shows how to combine a "View" and a
"Scope" plugin in the same application.


Build instruction
-----------------


### CMake 2.8.9 ###

Install CMake 2.8.9 or later. This is the minimum required version for
using Qt5, and therefore also for building QtYARP.

You can download CMake or later from the
[CMake website](http://www.cmake.org/cmake/resources/software.html) or
install it from Debian/Ubuntu repositories.

Ubuntu 12.04 is the only release supported by YARP that doesn't have a
CMake supported version, but you can still download it from
CMake website. For Debian Squeeze you will need to enable the
backports repository.


### Qt 5.2 ###

Install Qt 5.2 or later. QtYARP uses the QtQuick Controls plugin that
is available only since 5.2.

You can download  from your distribution packages or from the
[Qt Project website](http://qt-project.org/downloads) or from
Debian/Ubuntu repositories.

On Debian testing you need to install the packages:

   - qtbase5-dev
   - qtdeclarative5-dev
   - qtmultimedia5-dev
   - qml-module-qtquick2
   - qml-module-qtquick-window2
   - qml-module-qtmultimedia
   - qml-module-qtquick-dialogs
   - qml-module-qtquick-controls

Please note that since the Qt5 Debian packaging is quite new, older
releases do not have these packages and on some Ubuntu releases the name of the
packages could be slightly different.

If you installed Qt5 from the [Qt Project website][qt-project], you will
need to set up the environment variable `Qt5_DIR` pointing to the location where Qt5 cmake modules are, e.g. for qt 5.3:

`
export Qt5_DIR=path_to_Qt/5.3/gcc_64/lib/cmake/Qt5/
`

Alternatively you can add the `CMAKE_PREFIX_PATH` environment variable to find Qt5
cmake modules.

You may get this error while running cmake:

`
The imported target "Qt5::Gui" references the file "Qt5Gui_EGL_LIBRARY-NOTFOUND"
`

This is due to a problem with the installation script (check: http://qt-project.org/forums/viewthread/44605). 
A workaround is to install libegl1-mesa-dev:

`
apt-get install libegl1-mesa-dev
`

##### Windows #####
Download and install precompiled binaries from the
[Qt Project website](http://qt-project.org/downloads).

Make sure you define the following environment variables:

`
Qt5_DIR= location where cmake modules are, e.g. C:\Qt\5.3\msvc2010_opengl
`

You need also to ensure Qt is in the system path, i.e. check that your PATH includes Qt's binaries and DLLs:

`
PATH= ... ... ;C:\Qt\5.3\msvc2010_opengl\bin
`

In some configurations we found that you have to make the environment variable `CMAKE_PREFIX_PATH` point to Windows SDK location. For example, for a Windows 7 platform, the following may hold:

`CMAKE_PREFIX_PATH=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A`

### YARP 2.3.62.1 (master) ###

Download and build YARP from the master branch from
[YARP GitHub Repository](https://github.com/robotology/yarp). Previous
versions do not install the `YARP_manager` library and cannot be used.

In order to build the manager, you will have to build YARP from master
and enable the `YARP_EXPERIMENTAL_MANAGER_LIB` flag in CMake.


### Build ###

Build like a standard CMake project:

```
mkdir build
cd build
cmake ..
make
```
