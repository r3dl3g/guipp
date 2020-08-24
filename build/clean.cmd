@echo off

call get_versions.cmd

pushd %TARGET_DIR%

msbuild IT3DCamCtrlPrj.sln /m /p:Configuration=Debug /t:Clean
msbuild IT3DCamCtrlPrj.sln /m /p:Configuration=Release /t:Clean

popd
