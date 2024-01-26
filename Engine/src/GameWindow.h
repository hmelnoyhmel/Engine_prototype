#pragma once
#include <memory>
#include <Windows.h>
#include <string>

class DirectDevice;
class DirectSwapChain;

class GameWindow
{
public:
	GameWindow(HINSTANCE hInstance, DirectDevice& device, unsigned int width, unsigned int height);
	virtual LRESULT ProcessMessage(unsigned int msg, WPARAM wParam, LPARAM lParam);
	void SetWindowName(std::wstring windowName) const;

	void Render();

	inline HWND GetHWND() const { return nativeHwnd; }
	inline std::shared_ptr<DirectSwapChain> getSwapChain() const { return swapChain; }

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	float GetAspectRatio() const;

private:
	std::wstring windowName = L"";
	HWND nativeHwnd;
	DirectDevice& device;

	std::shared_ptr<DirectSwapChain> swapChain;
	void CreateWindowSwapChain(DirectDevice& device, int width, int height);

};
