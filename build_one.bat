@echo off

if {%2}=={} call "%VS80COMNTOOLS%\vsvars32.bat"

set binfolder=%~dp0%..\bin
set tempfolder=%~dp0%..\tmp
set outname=%~n1%

if not exist %binfolder% mkdir %binfolder%
if not exist %tempfolder% mkdir %tempfolder%

@echo on
cl /EHsc /Fo"%tempfolder%\%outname%" /Fe"%binfolder%\%outname%.exe" "%1%"