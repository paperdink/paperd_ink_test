@echo off && setlocal enabledelayedexpansion 
for /f "tokens=2delims=COM:" %%i in ('mode^|findstr /C:"COM"')do set "_com=%%i"
echo ===================
echo Uploading to COM%_com%
echo ===================

esp32.bat COM%_com% merlot
