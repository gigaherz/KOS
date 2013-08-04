@echo off
del success.txt

set OUTPATH=%1
set OUTPUT=%OUTPATH%\Output.flp
set TARGET=%2
set TARGETFN=%~nx2
set TOOLS=%3

copy "%OUTPATH%\Image\BootableFloppy.flp" "%OUTPUT%"
IF ERRORLEVEL 1 GOTO error

"%TOOLS%\MultibootFix.exe" "%TARGET%"
IF ERRORLEVEL 1 GOTO error

"%TOOLS%\Fatten.exe" "%OUTPUT%" /add "%TARGET%" %TARGETFN%
IF ERRORLEVEL 1 GOTO error

echo 1 > success.txt
GOTO end

:error
echo ERROR executing post-build operations.

:end