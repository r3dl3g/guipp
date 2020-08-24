@echo off

call get_versions.cmd

set BUILD_DIR=%BUILD_DIR%-static

mkdir %BUILD_DIR%\gui
pushd %BUILD_DIR%\gui
cmake "..\..\.." -G"%GENERATOR%" -DCMAKE_CONFIGURATION_TYPES=Debug;Release -DGUIPP_BUILD_STATIC_MODULE_LIBS=ON
popd

mkdir %BUILD_DIR%\Test
pushd %BUILD_DIR%\Test
cmake "..\..\..\test" -G"%GENERATOR%" -DCMAKE_CONFIGURATION_TYPES=Debug;Release -DGUIPP_BUILD_STATIC_MODULE_LIBS=ON
popd
