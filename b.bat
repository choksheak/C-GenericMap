@echo off

set TARGET=TestMain.exe

if "%1" == "clean" goto :clean

:exe

echo Build %TARGET%
cd src
cl *.c /link /out:%TARGET%
set exitcode=%ERRORLEVEL%
cd ..
exit /b %exitcode%

:clean

echo Clean files
if exist src\*.obj del src\*.obj
if exist src\%TARGET% del src\%TARGET%
if exist src\ut del src\ut
exit /b
