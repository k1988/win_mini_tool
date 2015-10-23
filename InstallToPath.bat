@echo off
@cd %~dp0%
@%~d0%
@set regpath="HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Environment"

for,/f,"skip=4 tokens=1,2,*",%%a,in,('reg query %regpath% /v Path'),do,(
        echo 原环境变量:
        :echo %%a :path
        :echo %%b :注册表类型
        echo %%c
	echo;
	:FIXME TODO 添加判断当前path是否已经有了当前目录
	echo 更改后环境变量
	echo %%c;%CD%;%CD%\bin;%CD%\bats;%CD%\scripts
        reg add %regpath% /v Path /t REG_EXPAND_SZ /d "%%c;%CD%;%CD%\bin;%CD%\bats;%CD%\scripts;" /f
	goto :eof
)

:eof
pause