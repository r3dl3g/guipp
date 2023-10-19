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
cmake "%PRJ_DIR%" -G"%GENERATOR%" -DCMAKE_CONFIGURATION_TYPES=Debug;Release -DGUIPP_BUILD_DEPENDENT_LIBS=ON -DGUIPP_SAMPLES=ON -DGUIPP_TESTS=ON -DGUIPP_TOOLS=ON -DLOGGING_BUILD_STATIC_MODULE_LIB=ON -DUTIL_BUILD_STATIC_MODULE_LIB=ON %2 %3 %4 %5 %6 %7 %8 %9
popd
endlocal
