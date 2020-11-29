@echo off

call get_versions.cmd

pushd %BUILD_DIR%

msbuild gui-lib.sln /m /p:Configuration=Debug /t:Clean
msbuild gui-lib.sln /m /p:Configuration=Release /t:Clean

popd
