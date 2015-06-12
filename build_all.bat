call "%VS80COMNTOOLS%\vsvars32.bat"
for /r %%i in (.\src\*.cpp) do .\build_one.bat "%%i" true