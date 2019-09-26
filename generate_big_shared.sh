
builddir=build/$(c++ --version | { read first rest ; echo $first ; })$(c++ -dumpversion)-big-shared
prjdir=$PWD

mkdir -p $builddir/debug
pushd $builddir/debug
cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DBASEPP_BUILD_STATIC_MODULE_LIBS=ON -DGUIPP_BUILD_STATIC_MODULE_LIBS=OFF -DLOGGING_BUILD_STATIC_MODULE_LIB=ON -DGUIPP_BUILD_SHARED_MODULE_LIBS=OFF
popd

mkdir -p $builddir/release
pushd $builddir/release
cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Release" -DBASEPP_BUILD_STATIC_MODULE_LIBS=ON -DGUIPP_BUILD_STATIC_MODULE_LIBS=OFF -DLOGGING_BUILD_STATIC_MODULE_LIB=ON -DGUIPP_BUILD_SHARED_MODULE_LIBS=OFF
popd
