
builddir=build/gcc-$(gcc -dumpversion)
prjdir=$PWD

mkdir -p $builddir/debug
pushd $builddir/debug
cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug"
popd

mkdir -p $builddir/release
pushd $builddir/release
cmake "$prjdir" -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Release"
popd
