#include <windows.h>
#include "memset.h"

//tcc -nostdlib min.c -lkernel32
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {


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

extern void _start()
{
  
  MSG msg1;
  WNDCLASS wc1;
  HWND hWnd1;

  // Example application code
  MessageBox(NULL, "Hello, Windows API!", "WinMain Entry Point", MB_OK);
    
  ZeroMemory(&wc1, sizeof wc1);
  wc1.hInstance = GetModuleHandle(NULL);
  wc1.lpszClassName = AppName;
  wc1.lpfnWndProc = (WNDPROC)WndProc;
  wc1.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
  wc1.hbrBackground = CreateSolidBrush(RGB(128, 128, 128));
  wc1.hIcon = LoadIcon(NULL, IDI_INFORMATION);
  wc1.hCursor = LoadCursor(NULL, IDC_ARROW);
  MessageBox(NULL, "1", "TEST", NULL);
  if(RegisterClass(&wc1) == FALSE) 
  {
    MessageBox(NULL, "b2", "TEST", NULL);
    goto end;
  }
    MessageBox(NULL, "2", "TEST", NULL);
  hWnd1 = CreateWindow(AppName, AppName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 10, 10, 360, 240, 0, 0, wc1.hInstance, 0);
    MessageBox(NULL, "3", "TEST", NULL);
  if(hWnd1 == NULL){
MessageBox(NULL, "b3", "TEST", NULL);
    goto end;
  }
    MessageBox(NULL, "4", "TEST", NULL);
  while(GetMessage(&msg1,NULL,0,0) > 0){
    TranslateMessage(&msg1);
    DispatchMessage(&msg1);
  }
end:
    // Exit the application without using the C runtime
    ExitProcess(0);
}

