@echo off

call get_versions.cmd

pushd %BUILD_DIR%\static

msbuild guipp.sln /m /p:Configuration=Debug /t:Clean
msbuild guipp.sln /m /p:Configuration=Release /t:Clean

popd
