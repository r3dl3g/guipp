
builddir=./$(c++ --version | { read first rest ; echo $first ; })$(c++ -dumpversion)
prjdir=$PWD/..

#mkdir -p $builddir/xcode
#pushd $builddir/xcode
#cmake "$prjdir" -G"Xcode" -DCMAKE_CONFIGURATION_TYPES=Debug;Release -DGUIPP_USE_QT=ON
#popd

mkdir -p $builddir/debug
pushd $builddir/debug
cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DGUIPP_USE_QT=ON -DCMAKE_PREFIX_PATH=/usr/local/opt/qt5
popd

#mkdir -p $builddir/release
#pushd $builddir/release
#cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Release"
#popd
