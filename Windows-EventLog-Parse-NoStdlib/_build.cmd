REM TCC 9.27
set TCC_EXE=C:\local\tcc32\tcc.exe

%TCC_EXE% -impdef %SYSTEMROOT%\system32\advapi32.dll
%TCC_EXE% logparse.c advapi32.def -luser32
