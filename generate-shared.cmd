@echo off

call get_versions.cmd

set BUILD_DIR=%BUILD_DIR%-shared

mkdir %BUILD_DIR%\gui
pushd %BUILD_DIR%\gui
cmake "..\..\.." -G"%GENERATOR%" -DCMAKE_CONFIGURATION_TYPES=Debug;Release -DGUIPP_BUILD_SHARED_MODULE_LIBS=ON
popd

#goto end
#
#mkdir %BUILD_DIR%\Test
#pushd %BUILD_DIR%\Test
#cmake "..\..\..\test" -G"%GENERATOR%" -DCMAKE_CONFIGURATION_TYPES=Debug;Release -DGUIPP_BUILD_SHARED_MODULE_LIBS=ON
#popd
#
:end