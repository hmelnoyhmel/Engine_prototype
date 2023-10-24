#pragma once
#include <windows.h>

class UserApp;

class WindowsSystem
{
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static int Run(UserApp& app);
};