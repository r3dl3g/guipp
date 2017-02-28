@echo off

set BUILD_DIR=build
set GENERATOR=Visual Studio

if "%VisualStudioVersion%" == "14.0" (
	echo Visual Studio Version 2015
	set BUILD_DIR=%BUILD_DIR%-vc14
	set GENERATOR=%GENERATOR% 14 2015 Win64
) else (
	if "%VisualStudioVersion%" == "12.0" (
		echo Visual Studio Version 2013
		set BUILD_DIR=%BUILD_DIR%-vc12
		set GENERATOR=%GENERATOR% 12 2013 Win64
	) else (
		if NOT "%VisualStudioVersion%" == "" (
			echo Unbekannte Visual Studio Version '%VisualStudioVersion%'!
		) else (
			echo Skript muss in einer Visual Studio Developer Eingabeaufforderung ausgefuehrt werden!
		)
		goto end
 	)
)

echo Build dir:%BUILD_DIR%
echo Generator:%GENERATOR%

:end
