@echo off

call get_versions.cmd

set BUILD_DIR=%BUILD_DIR%-%CLANG_TOOLSET%

mkdir %BUILD_DIR%\gui
pushd %BUILD_DIR%\gui
cmake --trace "..\..\.." -G"%GENERATOR%" -T"%CLANG_TOOLSET%" -DCMAKE_CONFIGURATION_TYPES=Debug;Release 
popd

goto end

mkdir %BUILD_DIR%\Test
pushd %BUILD_DIR%\Test
cmake "..\..\..\test" -G"%GENERATOR%" -T"%CLANG_TOOLSET%" -DCMAKE_CONFIGURATION_TYPES=Debug;Release 
popd

:end
