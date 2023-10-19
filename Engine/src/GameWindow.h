#pragma once
#include "Win32.h"


class GameWindow
{
private:
	unsigned int m_windowWidth;
	unsigned int m_windowHeight;
	HWND m_hwnd;

public:
	GameWindow(HWND hwnd, unsigned int width, unsigned int height);

	LRESULT ProcessMessage(unsigned int msg, WPARAM wParam, LPARAM lParam);
};
