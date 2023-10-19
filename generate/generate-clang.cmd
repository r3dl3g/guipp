@echo off

call get_versions.cmd

call generate.cmd clang "-T%CLANG_TOOLSET%"
call generate.cmd clang-shared "-T%CLANG_TOOLSET%" "-DGUIPP_BUILD_SHARED_MODULE_LIBS=ON"
call generate.cmd clang-static "-T%CLANG_TOOLSET%" "-DGUIPP_BUILD_STATIC_MODULE_LIBS=ON"
