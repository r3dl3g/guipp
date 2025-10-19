@echo off

call .project_defines.cmd

%BUILD_RELEASE% %TEE% %~n0.log
%BUILD_DEBUG% %TEE% %~n0.log
