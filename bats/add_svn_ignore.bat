@echo off
rem 此批处理的用途

rem 参考链接：http://stackoverflow.com/questions/17298668/svn-ignore-like-gitignore
rem 参考命令行
rem svn propset svn:ignore -RF /root/svn-ignore.txt . [dot for current dir]
rem 各语言通用

set txt_path=%~dp0%
svn propset svn:global-ignores -F "%txt_path%svn_ignored_common.txt" %cd%

rem java开发
svn propset svn:global-ignores -F "%txt_path%\svn_ignored_java.txt" %cd%

rem android studio
svn propset svn:ignore -F "%txt_path%\svn_ignored_android_studio.txt" %cd%
pause
