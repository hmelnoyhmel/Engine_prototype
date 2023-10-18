#pragma once
#include <windows.h>
#include <string>

class UserApp;

class Win32
{
public:
    static int Run(UserApp* app, HINSTANCE hInstance, int nCmdShow);
    static inline HWND GetHwnd() { return m_hwnd; }

private:
    static inline HWND m_hwnd;

    static void CalculateFrameStats(UserApp* pApp);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};