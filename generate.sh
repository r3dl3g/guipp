mkdir -p build.gcc/gui
pushd build.gcc/gui
cmake "../.." -G"Unix Makefiles" -DCMAKE_CONFIGURATION_TYPES=Debug;Release
popd
mkdir -p build.gcc/Test
pushd build.gcc/Test
cmake "../../test" -G"Unix Makefiles" -DCMAKE_CONFIGURATION_TYPES=Debug;Release
popd
