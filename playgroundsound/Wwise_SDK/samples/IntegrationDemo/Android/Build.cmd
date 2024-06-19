@echo off
rem Audiokinetic Integration Demo Android Build Script
SETLOCAL
set tmpNDKROOT="%NDKROOT%"
set inDir=%~dp0
set inDir=%inDir:\=/%
set inDir=%inDir: =\ %

Echo Entering directory %inDir%
pushd %~dp0
rem Parse Parameters...
rem Looking for help?
if /I "%1" == "help" goto SHOW_USAGE
if /I "%1" == "-h" goto SHOW_USAGE
if /I "%1" == "-help" goto SHOW_USAGE
if /I "%1" == "--help" goto SHOW_USAGE
if /I "%1" == "/?" goto SHOW_USAGE
if /I "%1" == "/help" goto SHOW_USAGE
if /I "%1" == ""  goto SHOW_USAGE

if /I "%1" == "armeabi-v7a" (
    Set ANDROID_ARCH=armeabi-v7a
) else (
    if /I "%1" == "x86" (
        Set ANDROID_ARCH=x86
    ) else (
        if /I "%1" == "arm64-v8a" (
            Set ANDROID_ARCH=arm64-v8a
        ) else (
            if /I "%1" == "x86_64" (
                Set ANDROID_ARCH=x86_64
            ) else (
                Echo . error Invalid arch: %1
                goto END
            )
        )
    )
)

if /I "%2" == "debug" (
 Set CONFIG=debug
) else (
    if /I "%2" == "profile" (
     Set CONFIG=profile
    ) else (
        if /I "%2" == "release" (
         Set CONFIG=release
        ) else (
            Echo . error Invalid config: %2
            goto END
        )
    )
)

set NDKROOT=%NDKROOT:\=/%
set NDKROOT=%NDKROOT: =\ %
Echo New NDKROOT = %NDKROOT%

Set GRADLE_VARIANT=%CONFIG%_%ANDROID_ARCH%Debug
Set GRADLE_COMMAND=CALL gradlew --no-daemon publish%GRADLE_VARIANT%

Echo Building...
Echo %GRADLE_COMMAND%
%GRADLE_COMMAND%
IF ERRORLEVEL 1 GOTO errorHandling

popd
goto END
:SHOW_USAGE
echo.
echo Audiokinetic Integration Demo Android Build Script
echo.
echo    Usage: Build.cmd [armeabi-v7a^|x86^|arm64-v8a^|x86_64] [Debug^|Profile^|Release]
echo.
echo    Examples:
echo       Build.cmd armeabi-v7a debug
echo.
goto END

REM no error here, errolevel == 0
:errorHandling
echo Error, aborting.
:END
set NDKROOT=tmpNDKROOT
