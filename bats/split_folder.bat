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

rem 手动设置日期过滤
set /a firstdate=20170317
set /a lastdate=20170322

rem 每一页条数
set page_count=500

rem 起始页码
set cur_page=0

rem 起始页内索引
set index=0

SETLOCAL ENABLEDELAYEDEXPANSION 

rem 循环处理列举出来的文件，如有特殊需求，可改下面的copy语句
for /f "delims=" %%i in ('dir /b /s /a-d %src%') do (
    set createdate=%%~ti
     set cdate=!createdate:~0,4!!createdate:~5,2!!createdate:~8,2!
     if !cdate! GEQ %firstdate% (
          if !cdate! LSS %lastdate% (
               set /a index = !index! + 1
            set /a cur_page = !index! / %page_count%
            if not exist %dest%\!cur_page! mkdir %dest%\!cur_page!
               echo %%~si 符合条件,复制
               move "%%i" "%dest%\!cur_page!\%%~nxi"               
          ) else (
               echo %%~ni 创建时间过晚，过滤掉
          )
     ) else (
          echo %%~ni 创建时间过早，过滤掉
     )
)   

:end
pause