REM set TCC_EXE=C:\local\tcc32\tcc.exe

REM %TCC_EXE% shell.c sqlite3.c
REM
SET vcvars=C:\Program Files (x86)\Microsoft Visual Studio .NET 2003\Vc7\bin\vcvars32.bat
REM SET vcvars=C:\MSVC4\BIN\vcvars32.bat

call "%vcvars%" x86
REM cl shell.c sqlite3.c /link /FIXED:NO /subsystem:console,3.10 /ENTRY:_start
cl /O1 /Op /QIfist /DSQLITE_OMIT_FLOATING_POINT -U SQLITE_WIN32_HAS_WIDE shell.c sqlite3.c

set LIB=C:\MSVC4\LIB
rem C:\msvc4\BIN\LINK.EXE shell.obj sqlite3.obj /FIXED:NO /subsystem:console,3.10
LINK.EXE shell.obj sqlite3.obj kernel32.lib /FIXED:NO /subsystem:console,3.10
