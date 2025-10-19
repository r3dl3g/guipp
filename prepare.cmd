@echo off

call .project_defines.cmd

%PREPARE_RELEASE% %TEE% %~n0.log
%PREPARE_DEBUG% %TEE% %~n0.log
