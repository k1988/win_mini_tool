@dir *.ocx
@for %%i in (*.ocx) do regsvr32 /s "%%i"
@dir *.dll
@for %%i in (*.dll) do regsvr32 /s "%%i"
pause