
builddir=./$(c++ --version | { read first rest ; echo $first ; })$(c++ -dumpversion)
prjdir=$PWD/..

mkdir -p $builddir/xcode
pushd $builddir/xcode
cmake "$prjdir" -G Xcode -DGUIPP_USE_QT=ON -DCMAKE_PREFIX_PATH=/usr/local/opt/qt5
popd

#-DCMAKE_BUILD_TYPE="Release"
