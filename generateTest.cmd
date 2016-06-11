mkdir build\Tests
pushd build\Tests
cmake "..\..\Tests" -G"Visual Studio 12 2013 Win64" -DCMAKE_CONFIGURATION_TYPES=Debug;Release 
popd
