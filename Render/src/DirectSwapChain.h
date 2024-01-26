#pragma once
#include <wrl.h>
#include <dxgi1_6.h>

using Microsoft::WRL::ComPtr;

class DirectDevice;
struct ScreenResizeMessage;

class DirectSwapChain
{
public:
	DirectSwapChain(DirectDevice& device, HWND hwnd, const unsigned int width, const unsigned int height);
	void Resize(const unsigned int newWidth, const unsigned int newHeight);
	void Present() const;

	inline unsigned int GetWidth() const { return width; }
	inline unsigned int GetHeight() const { return height; }
	inline ComPtr<IDXGISwapChain3> GetNativeSwapChain() const { return nativeSwapChain; }


private:

	ComPtr<IDXGISwapChain3> nativeSwapChain;
	static constexpr unsigned int swapChainBufferCount = 3;
	DirectDevice& device;
	HWND hwnd;
	unsigned int width;
	unsigned int height;

};