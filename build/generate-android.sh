
CXX=android-clang

export ANDROID_SDK=/home/armin/Android/Sdk
export ANDROID_NDK=$ANDROID_SDK/ndk/21.3.6528147
export QTDIR=/home/armin/Android/qt-5.15.2
export JAVA_HOME=`dirname $(dirname $(readlink -f $(which javac)))`

TOOL_CHAIN=$ANDROID_NDK/build/cmake/android.toolchain.cmake


Qt5Gui_DIR=$QTDIR/lib/cmake/Qt5Gui
Qt5Core_DIR=$QTDIR/lib/cmake/Qt5Core

. ./.generate.sh

#generate android-release-static Release -DGUIPP_BUILD_STATIC_MODULE_LIBS=ON \

generate_all android \
-DGUIPP_STANDALONE_BUILD=ON \
-DGUIPP_USE_QT=ON \
-DGUIPP_BUILD_FOR_MOBILE=ON \
-DANDROID_ABI=armeabi-v7a \
-DANDROID_ARM_NEON=TRUE \
-DANDROID_NATIVE_API_LEVEL=29 \
-DANDROID_STL=c++_shared \
-DANDROID_TOOLCHAIN=clang \
-DCMAKE_TOOLCHAIN_FILE=$TOOL_CHAIN \
-DCMAKE_CXX_PLATFORM_ID=$CMAKE_CXX_PLATFORM_ID \
-DQt5Gui_DIR=$Qt5Gui_DIR \
-DQt5Core_DIR=$Qt5Core_DIR \
$*
