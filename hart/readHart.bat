@echo off
for /f "tokens=1,2 delims==" %%a in (../platformio.ini) do ( if %%a==kitId set kitId=%%b)
CALL udpserial.exe inindkit%kitId%.local 400%kitId% CNCA1%kitId%

