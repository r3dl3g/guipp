@echo off

call get_versions.cmd

set BUILD_DIR=%BUILD_DIR%\static

pushd %BUILD_DIR%
cmake --build . --config Debug -- /m:6
cmake --build . --config Release -- /m:6
popd
