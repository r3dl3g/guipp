@echo off

call get_versions.cmd

pushd %BUILD_DIR%-shared/gui
cmake --build . --config Debug -- /m:6
cmake --build . --config Release -- /m:6
popd
