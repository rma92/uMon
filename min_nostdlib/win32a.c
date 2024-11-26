#include <windows.h>
/*make for VC++
 cl /c /O1 win32a.c /link user32.lib kernel32.lib /SUBSYSTEM:Windows /ENTRY:_main /ALIGN:16


 tcc32\tcc -c win32a.c -o win32ax.obj
 cl /c /O1 win32a.c 
REM link /nologo /SUBSYSTEM:Windows /ENTRY:_main /ALIGN:16 win32a.obj user32.lib kernel32.lib
 link /nologo /OSVERSION:3.1 /SUBSYSTEM:Windows /ENTRY:_main win32a.obj user32.lib kernel32.lib
 link /nologo /SUBSYSTEM:Windows /ENTRY:_main /ALIGN:16 win32ax.obj user32.lib kernel32.lib
 
Visual C++ 2.0
SET PATH=C:\MSVC20\BIN
SET LIB=C:\MSVC20\LIB
SET INCLUDE=C:\MSVC20\INCLUDE
 link /nologo /OSVERSION:3.1 /SUBSYSTEM:Windows /ENTRY:_main win32a.obj user32.lib kernel32.lib
 (compiling it

*/
  void *__cdecl memset(void *_Dst,int _Val,size_t _Size)
{
  return NULL;
}

LPCSTR AppName = "TestApp";
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch(message)
  {
    case WM_CREATE:
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
  MSG msg1;
  WNDCLASS wc1;
  HWND hWnd1;
  ZeroMemory(&wc1, sizeof wc1);
  wc1.hInstance = hInst;
  wc1.lpszClassName = AppName;
  wc1.lpfnWndProc = (WNDPROC)WndProc;
  wc1.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
  //wc1.hbrBackground = CreateSolidBrush(RGB(128, 128, 128));
  //wc1.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
  wc1.hIcon = LoadIcon(NULL, IDI_INFORMATION);
  wc1.hCursor = LoadCursor(NULL, IDC_ARROW);
  if(RegisterClass(&wc1) == FALSE) return 0;
  hWnd1 = CreateWindow(AppName, AppName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 10, 10, 360, 240, 0, 0, hInst, 0);
  if(hWnd1 == NULL) return 0;
  while(GetMessage(&msg1,NULL,0,0) > 0){
    TranslateMessage(&msg1);
    DispatchMessage(&msg1);
  }
  return msg1.wParam;
}

extern void _start()
{
  MessageBoxA(0, "T", "A", 0);
  WinMain(GetModuleHandle(NULL), NULL, "exe.exe", 4);
  ExitProcess(0);
}

extern void _winstart()
{
  WinMain(GetModuleHandle(NULL), NULL, "exe.exe", 4);
  ExitProcess(0);
}

