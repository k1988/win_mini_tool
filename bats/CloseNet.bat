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
rem ֹͣ��������������

@rem bat�в��������������ӵ�����
for /f "skip=2 tokens=3* delims= " %%a in ('netsh interface show interface') do (
	echo,%%b	
    netsh interface set interface "%%b" admin=disabled
)

rem ��ʾ�û��������
echo ����������������ֹͣ��

timeout /t 3 /nobreak

rem ��ȡ�����������б�
for /f "skip=2 tokens=3* delims= " %%a in ('netsh interface show interface') do (
	echo,%%b	
    netsh interface set interface "%%b" admin=enable
)
echo ���������������ѻָ���
pause

@rem name��admin����Ҫ�ӿո����
@rem netsh interface set interface name= �������� admin= enable
@rem ����Ϊdhcp�Զ���ȡip
@rem netsh interface ip set dns name = �������� source = dhcp


@echo off