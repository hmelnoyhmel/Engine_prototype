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
	// Native
	ComPtr<IDXGISwapChain3> m_swapChain;
	static constexpr unsigned int SwapChainBufferCount = 2;
	unsigned int m_width;
	unsigned int m_height;

	unsigned int m_currentFrameIndex;


	// Custom
	DirectDevice& r_device;



public:
	DirectSwapChain(DirectDevice& device);

	void CreateSwapChain();
	void SetResolution(ScreenResizeMessage msg);
};