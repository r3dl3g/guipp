#!/bin/bash

export CXX=em++

export QTDIR=/home/armin/Emscripten/qt-5.15.2
export JAVA_HOME=`dirname $(dirname $(readlink -f $(which javac)))`

#TOOL_CHAIN=$ANDROID_NDK/build/cmake/android.toolchain.cmake

Qt5Gui_DIR=$QTDIR/lib/cmake/Qt5Gui
Qt5Core_DIR=$QTDIR/lib/cmake/Qt5Core

. ./.generate.sh

#generate_all em++ \

generate em++-release-static Release -DGUIPP_BUILD_STATIC_MODULE_LIBS=ON \
-DGUIPP_STANDALONE_BUILD=ON \
-DGUIPP_USE_QT5=ON \
-DGUIPP_BUILD_FOR_MOBILE=ON \
-DQt5Gui_DIR=$Qt5Gui_DIR \
-DQt5Core_DIR=$Qt5Core_DIR \
-DVERBOSE=1
$*

#-DCMAKE_CXX_PLATFORM_ID=$CMAKE_CXX_PLATFORM_ID \
#-DCMAKE_TOOLCHAIN_FILE=$TOOL_CHAIN \
