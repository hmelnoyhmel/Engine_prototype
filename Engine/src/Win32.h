#pragma once
#include <memory>
#include <windows.h>
#include <string>
#include <unordered_map>

class UserApp;
class GameWindow;

class Win32
{
public:
    static int Run(UserApp* app, HINSTANCE hInstance, int nCmdShow);
    static inline HWND GetHwnd() { return m_hwnd; }

private:
    static inline HWND m_hwnd;
    static inline std::unordered_map<HWND, std::shared_ptr<GameWindow>> windowsMap;

    static void CalculateFrameStats(UserApp* pApp);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};