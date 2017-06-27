@echo off
aapt dump badging %1 %2 %3 %4 | findstr version
pause