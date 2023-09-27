@dir *.ocx
@for %%i in (*.ocx) do regsvr32 /s "%%i"
@dir *.dll
@for %%i in (*.dll) do regsvr32 /s "%%i"
pause

@rem 在命令行中可以这么执行
@rem for %i in (*.ocx) do regsvr32 /s "%i"
@rem for %i in (*.ocx) do regsvr32 /s "%i"