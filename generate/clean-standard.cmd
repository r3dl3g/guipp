@echo off

call get_versions.cmd

pushd %BUILD_DIR%\standard

msbuild guipp.sln /m /p:Configuration=Debug /t:Clean
msbuild guipp.sln /m /p:Configuration=Release /t:Clean

popd
