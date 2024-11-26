SET vcvars=C:\MSVC4\BIN\VCVARS32.BAT
CALL %vcvars% x86
del win32a_vc4.obj
cl /c /O1 win32a.c
ren win32a.obj win32a_vc4.obj
 link /nologo /NODEFAULTLIB /subsystem:windows,3.10 /OSVERSION:3.1 /ENTRY:_start win32a_vc4.obj user32.lib kernel32.lib /OUT:win32a_vc4_nt31.exe

