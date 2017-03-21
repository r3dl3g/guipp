mkdir -p build.gcc540.debug
cd build.gcc540.debug
cmake ".." -G"CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug"
cd ..
mkdir -p build.gcc540.release
cd build.gcc540.release
cmake ".." -G"CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE="Release"
cd ..
