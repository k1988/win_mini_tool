@set process_name=%1%
@if {%process_name%} == {} (
@echo Usage: getcmd chrome.exe
) else (
@wmic process where caption="%process_name%" get caption,commandline
)