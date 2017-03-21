@echo off 
rem 
set usage=Usage：split_folder 源文件夹(可加通配符) 目标文件夹

rem 待拆分的源文件夹
set src=%1%

rem 拆分的目标文件夹
set dest=%2%

rem 检查参数
if {%src%} == {} echo "%usage%" && goto end
if {%dest%} == {} echo "%usage%" && goto end

rem 每一页条数
set page_count=2

rem 起始页码
set cur_page=0

rem 起始页内索引
set index=0

SETLOCAL ENABLEDELAYEDEXPANSION 

rem 循环处理列举出来的文件，如有特殊需求，可改下面的copy语句
for /f "delims=" %%i in ('dir /b /s /a-d %src%') do (
    set /a index = !index! + 1
    set /a cur_page = !index! / %page_count%
    if not exist %dest%\!cur_page! mkdir %dest%\!cur_page!
    
    copy "%%i" "%dest%\!cur_page!\%%~nxi" 
)   

:end
pause