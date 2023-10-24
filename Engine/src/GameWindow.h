#pragma once
#include <Windows.h>
#include <string>

class GameWindow
{
private:
	unsigned int m_windowWidth;
	unsigned int m_windowHeight;
	std::wstring m_windowName = L"";
	HWND m_hwnd;

public:
	GameWindow(HINSTANCE hInstance, unsigned int width, unsigned int height);
	virtual LRESULT ProcessMessage(unsigned int msg, WPARAM wParam, LPARAM lParam);
	void SetWindowName(std::wstring windowName) const;
	void ShowGameWindow();
	inline HWND GetHWND() const { return m_hwnd; }
};