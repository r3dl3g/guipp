@echo off

call get_versions.cmd

call generate.cmd static "-DGUIPP_BUILD_STATIC_MODULE_LIBS=ON -DLOGGING_BUILD_STATIC_MODULE_LIB=ON -DUTIL_BUILD_STATIC_MODULE_LIB=ON"
