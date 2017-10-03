builddir=build/$(gcc -dumpversion)

mkdir -p $builddir/debug
pushd $builddir/debug
cmake "../../.." -G"CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug"
popd
mkdir -p $builddir/release
pushd $builddir/release
cmake "../../.." -G"CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE="Release"
popd
