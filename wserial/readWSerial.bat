@echo off
rem Execute para ler valores seriais pela porta UDP 
rem Se for do Simulador Workwi "inindkit%kitId%.local" por "localhost"
for /f "tokens=1,2 delims==" %%a in (../platformio.ini) do (if %%a==kitId set kitId=%%b)
com2tcp-rfc2217.bat \\.\CNCA1%kitId% inindkit%kitId%.local 400%kitId%