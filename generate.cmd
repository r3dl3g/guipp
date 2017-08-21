@echo off

call get_versions.cmd

mkdir %BUILD_DIR%\gui
pushd %BUILD_DIR%\gui
cmake "..\..\.." -G"%GENERATOR%" -DCMAKE_CONFIGURATION_TYPES=Debug;Release 
popd

mkdir %BUILD_DIR%\Test
pushd %BUILD_DIR%\Test
cmake "..\..\..\test" -G"%GENERATOR%" -DCMAKE_CONFIGURATION_TYPES=Debug;Release 
popd
