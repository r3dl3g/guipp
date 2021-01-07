@echo off

call get_versions.cmd

call generate.cmd clang "-T%CLANG_TOOLSET%"
