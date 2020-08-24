
builddir=./$(g++ --version | { read first rest ; echo $first ; })$(c++ -dumpversion)-shared
prjdir=$PWD/..

#mkdir -p $builddir/debug
#pushd $builddir/debug
#cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DUTIL_BUILD_STATIC_MODULE_LIB=OFF -DGUIPP_BUILD_STATIC_MODULE_LIBS=OFF -DLOGGING_BUILD_STATIC_MODULE_LIB=OFF -DGUIPP_BUILD_SHARED_MODULE_LIBS=ON
#popd

mkdir -p $builddir/release
pushd $builddir/release
cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Release" -DUTIL_BUILD_STATIC_MODULE_LIB=OFF -DGUIPP_BUILD_STATIC_MODULE_LIBS=OFF -DLOGGING_BUILD_STATIC_MODULE_LIB=OFF -DGUIPP_BUILD_SHARED_MODULE_LIBS=ON -DCMAKE_INSTALL_PREFIX=./dist
popd
