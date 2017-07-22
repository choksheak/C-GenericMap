@echo off

REM Fake makefile for Windows.

set OUTDIR=wout
set TARGET=TestMain.exe

if "%1" == "print" goto :print
if "%1" == "test" goto :test
if "%1" == "clean" goto :clean

:exe
    echo Build %TARGET%
    cd src
    cl *.c /link /out:%TARGET%
    set exitcode=%ERRORLEVEL%
    set outdir1=..\%OUTDIR%
    if not exist %outdir1% mkdir %outdir1%
    if exist "%TARGET%" move %TARGET% %outdir1%
    for %%F in (*.obj) do move %%F %outdir1%
    if exist src\ut del src\ut
    cd ..
    echo Exit code = %exitcode%
    exit /b %exitcode%

:print
    echo OUTDIR = %OUTDIR%
    echo TARGET = %TARGET%
    exit /b 0

:test
    if not exist %OUTDIR%\%TARGET% (
        echo %TARGET% not found. Please build the project first.
        exit /b 1
    )
    %OUTDIR%\%TARGET%
    set exitcode=%ERRORLEVEL%
    echo Exit code = %exitcode%
    exit /b %exitcode%

:clean
    echo Clean files
    if exist %OUTDIR% rd /s /q %OUTDIR%
    if exist src\ut del src\ut
    exit /b 0
