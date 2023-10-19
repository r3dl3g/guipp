@echo off

call get_versions.cmd

call generate.cmd static "-DGUIPP_BUILD_STATIC_MODULE_LIBS=ON"
