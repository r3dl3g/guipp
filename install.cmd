@echo off

call .project_defines.cmd

%INSTALL_RELEASE% %TEE% %~n0.log
%INSTALL_DEBUG% %TEE% %~n0.log
