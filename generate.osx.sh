
builddir=build/$(c++ --version | { read first rest ; echo $first ; })$(c++ -dumpversion)
prjdir=$PWD

mkdir -p $builddir
pushd $builddir
cmake "$prjdir" -G"Xcode" -DCMAKE_CONFIGURATION_TYPES=Debug;Release
popd

#mkdir -p $builddir/debug
#pushd $builddir/debug
#cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug"
#popd

#mkdir -p $builddir/release
#pushd $builddir/release
#cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Release"
#popd
