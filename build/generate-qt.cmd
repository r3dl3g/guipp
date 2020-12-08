@echo off

call get_versions.cmd

mkdir %BUILD_DIR%-qt
pushd %BUILD_DIR%-qt
cmake "..\.." -G"%GENERATOR%" -DGUIPP_USE_QT=ON -DCMAKE_PREFIX_PATH=D:\Dev\extern\qt-everywhere-opensource-src-5.9.5\qtbase -DCMAKE_CONFIGURATION_TYPES=Debug;Release
popd
