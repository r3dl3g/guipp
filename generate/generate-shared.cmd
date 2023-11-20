@echo off

call get_versions.cmd

call generate.cmd shared "-DGUIPP_BUILD_SHARED_MODULE_LIBS=ON -DLOGGING_BUILD_STATIC_MODULE_LIB=OFF -DUTIL_BUILD_STATIC_MODULE_LIB=OFF" %1 %2 %3 %4 %5 %6 %7 %8 %9
