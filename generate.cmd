mkdir build\gui
pushd build\gui
cmake "..\.." -G"Visual Studio 12 2013 Win64" -DCMAKE_CONFIGURATION_TYPES=Debug;Release 
popd
mkdir build\Test
pushd build\Test
cmake "..\..\test" -G"Visual Studio 12 2013 Win64" -DCMAKE_CONFIGURATION_TYPES=Debug;Release 
popd
