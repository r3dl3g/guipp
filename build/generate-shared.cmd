@echo off

call get_versions.cmd

call generate.cmd shared "-DGUIPP_BUILD_SHARED_MODULE_LIBS=ON"
