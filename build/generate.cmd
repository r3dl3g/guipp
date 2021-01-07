@echo off

pushd ..
set PRJ_DIR=%CD%
popd

call get_versions.cmd

set TARGET=%BUILD_DIR%\%1
setlocal enableextensions
if NOT EXIST %TARGET% md %TARGET%
pushd %TARGET%
echo Generate project files in %CD%
cmake "%PRJ_DIR%" -G"%GENERATOR%" -DCMAKE_CONFIGURATION_TYPES=Debug;Release %2 %3 %4 %5 %6 %7 %8 %9
popd
endlocal
