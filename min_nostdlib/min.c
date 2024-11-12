#include <windows.h>

//tcc -nostdlib min.c -lkernel32
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
void _start()
{
    // Example application code
    //MessageBox(NULL, "Hello, Windows API!", "WinMain Entry Point", MB_OK);
    
    // Exit the application without using the C runtime
    ExitProcess(0);
}

