REM this is not currently working
SET PATH=C:\MSVC20\BIN
SET LIB=C:\MSVC20\LIB
SET INCLUDE=C:\MSVC20\INCLUDE
cl /c /O1 win32a.c /o win32a.obj
 link /nologo /OSVERSION:3.1 /SUBSYSTEM:Windows /ENTRY:_main win32a.obj user32.lib kernel32.lib

