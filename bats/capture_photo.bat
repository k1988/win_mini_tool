@echo off

:: ����ͼƬ����Ŀ¼
set SAVE_PATH=%cd%

@if {"%1%"}=={""} (
echo δʹ�������д��ݱ���Ŀ¼,ʹ�õ�ǰĿ¼
:: ����ͼƬ����Ŀ¼
set SAVE_PATH=%~dp0%
) 

if not exist "%SAVE_PATH%" mkdir "%SAVE_PATH%"

:: ����ʱ���
set TIMESTAMP=%DATE:~0,4%%DATE:~5,2%%DATE:~8,2%_%TIME:~0,2%%TIME:~3,2%%TIME:~6,2%
set TIMESTAMP=%TIMESTAMP: =0%

@rem �豸�б�
@rem ffmpeg -list_devices true -f dshow -i dummy


:: ʹ�� FFmpeg ����
ffmpeg -f dshow -i video="Integrated Camera" -frames:v 1 "%SAVE_PATH%\photo_%TIMESTAMP%.jpg"

:: ɾ��7��ǰ��ͼƬ
echo on
forfiles /p "%SAVE_PATH%" /m *.jpg /d -7 /c "cmd /c del @path"
