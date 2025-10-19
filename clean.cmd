@echo off

call .project_defines.cmd

%CLEAN_RELEASE% %TEE% %~n0.log
%CLEAN_DEBUG% %TEE% %~n0.log
