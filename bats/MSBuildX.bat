
 @echo off

if {"%VSINSTALLDIR%"}=={""} (
goto check_vc
) else (
goto work
)

:check_vc
if exist "C:\Program Files\Microsoft Visual Studio\2023\Community\Common7\Tools\VsDevCmd.bat" echo "Using vc2022" && call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" && goto work
if exist "%VS140COMNTOOLS%1\vsvars32.bat" echo "Using vc140" && call "%VS140COMNTOOLS%\vsvars32.bat" && goto work
if exist "%VS80COMNTOOLS%\vsvars32.bat" echo "Using vc2005" && call "%VS80COMNTOOLS%\vsvars32.bat" && goto work

:work
 msbuild.exe /m /t:Build /p:Configuration=release_static /p:Platform=x64 %*