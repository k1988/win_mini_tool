@echo off

:: 设置图片保存目录
set SAVE_PATH=%cd%

@if {"%1%"}=={""} (
echo 未使用命令行传递保存目录,使用当前目录
:: 设置图片保存目录
set SAVE_PATH=%~dp0%
) 

if not exist "%SAVE_PATH%" mkdir "%SAVE_PATH%"

:: 设置时间戳
set TIMESTAMP=%DATE:~0,4%%DATE:~5,2%%DATE:~8,2%_%TIME:~0,2%%TIME:~3,2%%TIME:~6,2%
set TIMESTAMP=%TIMESTAMP: =0%

@rem 设备列表
@rem ffmpeg -list_devices true -f dshow -i dummy


:: 使用 FFmpeg 拍照
ffmpeg -f dshow -i video="Integrated Camera" -frames:v 1 "%SAVE_PATH%\photo_%TIMESTAMP%.jpg"

:: 删除7天前的图片
echo on
forfiles /p "%SAVE_PATH%" /m *.jpg /d -7 /c "cmd /c del @path"
