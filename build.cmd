@echo off

call get_versions.cmd

echo pushd %BUILD_DIR%\gui
pushd %BUILD_DIR%\gui
cmake --build . --config Debug -- /m:6
cmake --build . --config Release -- /m:6
popd
