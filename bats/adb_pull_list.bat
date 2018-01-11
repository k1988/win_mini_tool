@rem 使用adb pull list里每行路径对应的文件
@rem 格式见list.example.txt
@echo off
for /F "eol=#" %%i  in (list.txt) do (
echo %%i
adb pull /system/bin/%%i .
)
pause