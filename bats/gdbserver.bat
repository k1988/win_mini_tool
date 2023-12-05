@rem 将这段代码放在需要获取uac权限的bat前面
@echo off
NET SESSION >nul 2>&1 && goto noUAC
title.
set n=%0 %*
set n=%n:"=" ^& Chr(34) ^& "%
echo Set objShell = CreateObject("Shell.Application")>"%tmp%\cmdUAC.vbs"
echo objShell.ShellExecute "cmd.exe", "/c start " ^& Chr(34) ^& "." ^& Chr(34) ^& " /d " ^& Chr(34) ^& "%CD%" ^& Chr(34) ^& " cmd /c %n%", "", "runas", ^1>>"%tmp%\cmdUAC.vbs"
echo Not Admin, Attempting to elevate...
cscript "%tmp%\cmdUAC.vbs" //Nologo
del "%tmp%\cmdUAC.vbs"
exit /b
:noUAC

::-----Normal Batch Starts Here---------------------

set cur_folder=%~dp0%

tasklist | FindStr KKDRMain.exe
pause
set /p pid=请输入上面的 KKDRMain.exe 的进程id: 
%cur_folder%\gdbserver --once --attach 0.0.0.0:9876 %pid%
pause