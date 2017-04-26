@echo off
rem Android默认的文档是用file协议打开的，有些地方不太方便，就使用php开启个服务器来用浏览器打开
cd %ANDROID_HOME%\docs
D:
start cmd.exe /c php.exe -S 0.0.0.0:81
start http://127.0.0.1:81/reference/