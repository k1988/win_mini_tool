@echo off
del /s /q *.ncb
del /s /q *.obj
del /s /q *.sdf

del /s /a h *.suo
del /s *.user
del /s *.orig
del /s *.plg
del /s *.pch
del /s *.pdb

del /s /ah  /q *.suo
del /s *.old
del /s *.user
del /s *.orig
del /s *.plg
del /s *.ncb
del /s *.ilk
del /s *.pdb
del /s *.exp
del /s *.obj
:del /s */debug/*.htm
del /s *.dep
del /s *.idb
del /s *.pch

@FOR /r "%cd%" %%i IN (debug) Do  @RD /s /q "%%i" && @echo "%%iÒÑÉ¾³ý"
@FOR /r "%cd%" %%i IN (Debug) Do  @RD /s /q "%%i" && @echo "%%iÒÑÉ¾³ý"
@FOR /r "%cd%" %%i IN (release) Do  @RD /s /q "%%i" && @echo "%%iÒÑÉ¾³ý"
@FOR /r "%cd%" %%i IN (Release) Do  @RD /s /q "%%i" && @echo "%%iÒÑÉ¾³ý"
