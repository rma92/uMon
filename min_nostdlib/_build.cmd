REM TCC 9.27
set TCC_EXE=C:\local\tcc32\tcc.exe

REM Min.c
REM %TCC_EXE% -impdef %SYSTEMROOT%\system32\advapi32.dll
%TCC_EXE% -nostdlib min.c -lkernel32 min_tcc.exe

REM win32a.c
REM You can't make a Win32 application using MinGW's headers without a standard library because Winnt.h includes guiddef.h which includes string.h
REM to mitigate this, write an empty memset function.
%TCC_EXE% win32a.c -E > win32_pp.c
%TCC_EXE% win32a.c -nostdlib -o win32_tcc.exe -luser32 -lkernel32
REM %TCC_EXE% win32a.c -o win32_tcc_defaultstdlib.exe -luser32 -lkernel32

rem cmd /c start _build_vc2.cmd
cmd /c start cmd /k _build_vc4.cmd
