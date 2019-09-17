@echo off

call get_versions.cmd

mkdir -p %BUILD_DIR%
pushd %BUILD_DIR%
cmake "..\.." -G"%GENERATOR%" -DCMAKE_CONFIGURATION_TYPES=Debug;Release 
popd
