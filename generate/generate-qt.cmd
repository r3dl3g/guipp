@echo off

call get_versions.cmd

call generate.cmd qt-static "-DGUIPP_BUILD_STATIC_MODULE_LIBS=ON" "-DGUIPP_USE_QT=ON" "-DCMAKE_PREFIX_PATH=D:\Dev\extern\qt-everywhere-opensource-src-5.9.5\qtbase"
