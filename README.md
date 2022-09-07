# qt5-qml-networking

Implements Networking QML Types by provided QML wrappers to C++ networking components.

To use this in your project consider cloning this repo directly in your project:

    git clone https://github.com/stephenquan/qt5-qml-networking.git
  
or adding it as a submodule:

    git submodule add https://github.com/stephenquan/qt5-qml-networking.git
    git submodule update
  
Once added, and assume your Qt app is using qmake, you will need to make the following changes to your application's `project.pro` file:

```pro
CONFIG += qmltypes
QML_IMPORT_NAME = project
QML_IMPORT_MAJOR_VERSION = 1
include(qt5-qml-networking/qt5-qml-networking.pri)
```
