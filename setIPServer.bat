@echo off
set kitId=0
set port=47269
for /f "usebackq tokens=2 delims=:" %%f in (`ipconfig ^| findstr /c:"IPv4 Address"`) do echo ip:%%f port:%port%  | ncat -w10 inindkit%kitId%.local 4000

rem pkgmgr /iu:"TelnetClient" & telnet -a -e [ inindkit%kitId%.local 4000
rem ncat -u %kitName%.local 4000
rem pkgmgr /iu:"TelnetClient" & telnet %kitName%.local 4000