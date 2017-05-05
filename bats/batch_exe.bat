@echo off
cd %~dp0%
%~d0%
echo --------------------------------------------------  >> log.txt
echo %date% >> log.txt
for %%i in (*.exe) do (
"%%i" >> log.txt
if %errorlevel%==0 (
echo "执行 %%i 成功" >> log.txt
) else (
echo "执行 %%i 失败" >> log.txt
goto error
)
)
goto ok

:error
pause

:ok
