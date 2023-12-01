#pragma once
#include "Enums.h"
#include <wrl.h>
#include <dxgi1_6.h>

using Microsoft::WRL::ComPtr;

class DirectDevice;
struct ScreenResizeMessage;

class DirectSwapChain
{
private:

	ComPtr<IDXGISwapChain3> m_swapChain;
	static constexpr unsigned int SwapChainBufferCount = 3;
	DirectDevice& r_device;
	HWND m_hwnd;
	unsigned int m_width;
	unsigned int m_height;

public:
	DirectSwapChain(DirectDevice& device, HWND hwnd, unsigned int width, unsigned int height);
	void Resize(unsigned int width, unsigned int height);
	void Present();

	inline unsigned int GetWidth() const { return m_width; }
	inline unsigned int GetHeight() const { return m_height; }
};