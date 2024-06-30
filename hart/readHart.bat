@echo off
set kitId=0
set LIBSERIALPORT_PATH=.
CALL udpserial.exe inindkit%kitId%.local 400%kitId% CNCA%kitId%
rem for /f "usebackq tokens=2 delims=:" %%f in (`ipconfig ^| findstr /c:"IPv4 Address"`) do echo ip:%%f port:%port%  | ncat -w10 inindkit%kitId%.local 4000
