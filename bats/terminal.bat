:@echo off
@if {%1}=={} (set /p processName=输入要终止的进程名称，可以使用通配符) ELSE (set processName=%1%)
@if {%processName%}=={} exit
TASKKILL /F /FI "IMAGENAME eq %processName%"
pause