#pragma once
#include <windows.h>

class UserApp;

class WindowHub
{
public:
	static LRESULT CALLBACK HubWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static int Run(UserApp& app);
};