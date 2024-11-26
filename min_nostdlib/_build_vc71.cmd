REM SET vcvars=C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\bin\vcvars32.bat
SET vcvars=C:\Program Files (x86)\Microsoft Visual Studio .NET 2003\Vc7\bin\vcvars32.bat
CALL "%vcvars%" x86
cl /O1 win32a.c /link /nodefaultlib /FIXED:NO /subsystem:windows,3.10 /entry:_start kernel32.lib user32.lib gdi32.lib /out:win32_vc71_nt31.exe

cl /O1 min.c /link /nodefaultlib /FIXED:NO /subsystem:windows,3.10 /entry:_start kernel32.lib user32.lib /out:min_vc71_nt31.exe

cl /O1 min2.c /link /nodefaultlib /FIXED:NO /subsystem:windows,3.10 /entry:_start kernel32.lib user32.lib gdi32.lib /out:min2_vc71_nt31.exe

REM cl /O1 min2.c /link /nodefaultlib /FIXED:NO /subsystem:windows,3.10 /entry:_start kernel32.lib user32.lib gdi32.lib minicrt.lib /out:min2_vc71_nt31.exe
