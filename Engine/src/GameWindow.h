#pragma once
#include <memory>
#include <Windows.h>
#include <string>

class DirectDevice;
class DirectSwapChain;

class GameWindow
{
private:
	std::wstring m_windowName = L"";
	HWND m_hwnd;
	DirectDevice& m_device;

	std::shared_ptr<DirectSwapChain> m_windowSwapChain;
	void CreateWindowSwapChain(DirectDevice& device, int width, int height);

public:
	GameWindow(HINSTANCE hInstance, DirectDevice& device, unsigned int width, unsigned int height);
	virtual LRESULT ProcessMessage(unsigned int msg, WPARAM wParam, LPARAM lParam);
	void SetWindowName(std::wstring windowName) const;

	void Render();

	inline HWND GetHWND() const { return m_hwnd; }
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
};
