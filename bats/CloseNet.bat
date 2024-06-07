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

@echo off
rem 停止所有网络适配器

@rem bat中查找所有网络连接的名称
for /f "skip=2 tokens=3* delims= " %%a in ('netsh interface show interface') do (
	echo,%%b	
    netsh interface set interface "%%b" admin=disabled
)

rem 提示用户操作完成
echo 所有网络适配器已停止。

timeout /t 3 /nobreak

rem 获取网络适配器列表
for /f "skip=2 tokens=3* delims= " %%a in ('netsh interface show interface') do (
	echo,%%b	
    netsh interface set interface "%%b" admin=enable
)
echo 所有网络适配器已恢复。
pause

@rem name和admin后面要加空格才行
@rem netsh interface set interface name= 本地连接 admin= enable
@rem 设置为dhcp自动获取ip
@rem netsh interface ip set dns name = 本地连接 source = dhcp


@echo off