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
set binfolder=%~dp0%\bin
set tempfolder=%~dp0%\tmp
set outname=%~n1%

if not exist %binfolder% mkdir %binfolder%
if not exist %tempfolder% mkdir %tempfolder%

@echo on
@rem /D "WIN32" /D "_UNICODE" /D "UNICODE" /Zc:wchar_t 
cl /EHsc /std:c++17 /Fo"%tempfolder%\%outname%" /Fe"%binfolder%\%outname%.exe" "%1%"

@echo off
@rem 如果不是被build_all.bat批量调用，就暂停显示结果
if {%2}=={} pause
