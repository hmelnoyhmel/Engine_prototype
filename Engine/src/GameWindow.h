#pragma once
#include <memory>
#include <Windows.h>
#include <string>

#include "DirectConstantBuffer.h"
#include "MathHelper.h"
#include "SimpleMath.h"

class DirectMesh;
class DirectDevice;
class DirectSwapChain;

using namespace DirectX::SimpleMath;

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
	std::shared_ptr<DirectMesh> mesh;
	void CreateWindowSwapChain(DirectDevice& device, int width, int height);


	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;

	/*
	std::shared_ptr<DirectConstantBuffer> cameraCB;
	std::shared_ptr<DirectConstantBuffer> objectCB;



	struct alignas(sizeof(Vector4)) CameraData
	{
		Matrix Position;
		Matrix Rotation;
		Matrix View;
		Matrix Projection;
	};

	struct alignas(sizeof(Vector4)) ObjectData
	{
		Matrix Position;
		Matrix Rotation;
		Matrix Scale;
	};
	*/
};
