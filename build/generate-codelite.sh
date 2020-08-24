
builddir=./$(g++ --version | { read first rest ; echo $first ; })$(g++ -dumpversion)
prjdir=$PWD/..

mkdir -p $builddir/debug
pushd $builddir/debug
cmake "$prjdir" -G"CodeLite - Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_MAKE_PROGRAM="gmake"
popd

#mkdir -p $builddir/release
#pushd $builddir/release
#cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Release"
#popd
