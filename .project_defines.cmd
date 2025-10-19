@echo off

for %%I in (.) do set PROJECT=%%~nxI

set TEE=2^>^&1 ^> %TMP%\temp.log ^& type %TMP%\temp.log ^& type %TMP%\temp.log ^>^> 

WHERE /Q wtee
IF %ERRORLEVEL%==0 (
  set TEE=2^>^&1 ^| wtee -a 
)

set INSTALL_PREFIX=D:/install
set BUILD_PREFIX=D:/build
set BUILD_DIR=%BUILD_PREFIX%/%PROJECT%
set PARALLEL=%PROCESSOR_LEVEL%
set DEFINES=-DCMAKE_INSTALL_PREFIX=%INSTALL_PREFIX% -DCMAKE_PREFIX_PATH=%INSTALL_PREFIX%

if exist "%USERPROFILE%/.project_defines.cmd" (
  echo Loading user project_defines from "%USERPROFILE%/.project_defines.cmd"
  call "%USERPROFILE%/.project_defines.cmd"
) else (
  echo No user project_defines found at "%USERPROFILE%/.project_defines.cmd"
)

set PREPARE_RELEASE=echo. ^& echo ====== New Release prepare at %DATE% ======= ^& echo. ^& cmake -S. -B%BUILD_DIR%/Release -DCMAKE_CONFIGURATION_TYPES=Release %DEFINES%
set PREPARE_DEBUG=echo. ^& echo ====== New Debug prepare at %DATE% ======= ^& echo. ^& cmake -S. -B%BUILD_DIR%/Debug -DCMAKE_CONFIGURATION_TYPES=Debug %DEFINES%
set BUILD_RELEASE=echo. ^& echo ====== New Release build at %DATE% ======= ^& echo. ^& cmake --build %BUILD_DIR%/Release --parallel %parallel% --config Release
set BUILD_DEBUG=echo. ^& echo ====== New Debug build at %DATE% ======= ^& echo. ^& cmake --build %BUILD_DIR%/Debug --parallel %parallel% --config Debug
set INSTALL_RELEASE=echo. ^& echo ====== New Release install at %DATE% ======= ^& echo. ^& cmake --build %BUILD_DIR%/Release --target install --config Release
set INSTALL_DEBUG=echo. ^& echo ====== New Debug install at %DATE% ======= ^& echo. ^& cmake --build %BUILD_DIR%/Debug --target install --config Debug
set CLEAN_RELEASE=echo. ^& echo ====== New Release clean at %DATE% ======= ^& echo. ^& cmake --build %BUILD_DIR%/Release --target clean --config Release
set CLEAN_DEBUG=echo. ^& echo ====== New Debug clean at %DATE% ======= ^& echo. ^& cmake --build %BUILD_DIR%/Debug --target clean --config Debug
