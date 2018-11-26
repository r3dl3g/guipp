mkdir build\test
pushd build\test
cmake "..\..\test" -G"Visual Studio 12 2013 Win64" -DCMAKE_CONFIGURATION_TYPES=Debug;Release 
popd
