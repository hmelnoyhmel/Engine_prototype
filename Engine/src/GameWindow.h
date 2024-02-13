#pragma once
#include <memory>
#include <Windows.h>
#include <string>

#include <d3d12.h>
#include "SimpleMath.h"

class DirectMesh;
class DirectDevice;
class DirectSwapChain;

using namespace DirectX::SimpleMath;

class GameWindow
{
public:
	GameWindow(HINSTANCE hInstance, std::shared_ptr<DirectDevice> device, unsigned int width, unsigned int height);
	virtual LRESULT ProcessMessage(unsigned int msg, WPARAM wParam, LPARAM lParam);
	void SetWindowName(std::wstring windowName) const;

	void Render();

	void OnResize(const unsigned int newWidth, const unsigned int newHeight);

	inline HWND GetHWND() const { return nativeHwnd; }
	inline std::shared_ptr<DirectSwapChain> getSwapChain() const { return swapChain; }

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	float GetAspectRatio() const;

private:
	std::wstring windowName = L"";
	HWND nativeHwnd;
	std::shared_ptr<DirectDevice> device;

	std::shared_ptr<DirectSwapChain> swapChain;
	std::shared_ptr<DirectMesh> mesh;
	void CreateWindowSwapChain(std::shared_ptr<DirectDevice> device, int width, int height);

	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;
};
