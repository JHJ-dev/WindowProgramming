#include <windows.h>
#include <tchar.h>

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#define WIDTH 600
#define HEIGHT 800

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";
LPCTSTR lpszWindowName = "Window Program";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);