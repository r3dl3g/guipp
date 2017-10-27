DIR=build/clang
export CC=clang
export CXX=clang++
mkdir -p $DIR/debug
pushd $DIR/debug
cmake "../../.." -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_CXX_COMPILER=$CXX -DCMAKE_EXE_LINKER_FLAGS=-lstdc++
popd
mkdir -p $DIR/release
pushd $DIR/release
cmake "../../.." -G"Unix Makefiles" -DCMAKE_BUILD_TYPE="Release" -DCMAKE_CXX_COMPILER=$CXX -DCMAKE_EXE_LINKER_FLAGS=-lstdc++
popd
