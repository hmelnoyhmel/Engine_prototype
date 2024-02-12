#include "GameWindow.h"
#include "GameWindow.h"

#include "DirectSwapChain.h"
#include <DirectDevice.h>
#include <DirectXColors.h>

#include "DirectCommandList.h"
#include "DirectCommandQueue.h"
#include <SimpleMath.h>

#include "DirectResource.h"

#include "DirectMesh.h"

#include "DirectConstantBuffer.h"

using namespace DirectX::SimpleMath;

GameWindow::GameWindow(HINSTANCE hInstance, DirectDevice& device, unsigned int width, unsigned int height) :
	device{ device }
{
	nativeHwnd = CreateWindow(
		L"BasicWndClass", // Registered WNDCLASS instance to use.
		windowName.c_str(), // window title
		WS_OVERLAPPEDWINDOW, // style flags
		CW_USEDEFAULT, // x-coordinate
		CW_USEDEFAULT, // y-coordinate
		width, // width
		height, // height
		nullptr, // parent window
		nullptr, // menu handle
		hInstance, // app instance
		nullptr); // extra creation parameters

	SetWindowName(L"TestWindow");
	CreateWindowSwapChain(device, width, height);
	ShowWindow(nativeHwnd, SW_SHOW);
	UpdateWindow(nativeHwnd);

	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(width);
	mScreenViewport.Height = static_cast<float>(height);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };

	auto& q = device.GetCommandQueue(EQueueType::Graphics);
	auto list = device.GetCommandList(EQueueType::Graphics);

	mesh = std::make_shared<DirectMesh>(device, list);

	list.Close();
	q.ExecuteCommandList(list);


}

LRESULT GameWindow::ProcessMessage(unsigned int msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		auto const pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		SetWindowLongPtr(nativeHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		//MessageBox(nullptr, L"Hello, World", L"Hello", MB_OK);
		return 0;
	}
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
			DestroyWindow(nativeHwnd);
		return 0;
	}
	case WM_SIZE:
	{
		swapChain->Resize(LOWORD(lParam), HIWORD(lParam));
		std::wstring const text = windowName +
			L"    width: " + std::to_wstring(GetWidth()) +
			L"    height: " + std::to_wstring(GetHeight());
		SetWindowText(nativeHwnd, text.c_str());
		//m_windowSwapChain->Resize(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	default:
	{
		return 0;
	}
	}
}

void GameWindow::SetWindowName(std::wstring windowName) const
{
	SetWindowText(nativeHwnd, windowName.c_str());
}


void GameWindow::CreateWindowSwapChain(DirectDevice& device, int width, int height)
{
	swapChain = std::make_shared<DirectSwapChain>(device, nativeHwnd, width, height);
}

unsigned int GameWindow::GetWidth() const
{
	return swapChain->GetWidth();
}

unsigned int GameWindow::GetHeight() const
{
	return swapChain->GetHeight();
}

float GameWindow::GetAspectRatio() const
{
	return static_cast<float>(GetWidth()) / static_cast<float>(GetHeight());
}


void GameWindow::Render()
{
	auto& q = device.GetCommandQueue(EQueueType::Graphics);
	auto list = device.GetCommandList(EQueueType::Graphics);

	mesh->Update(list);

	const auto sc = swapChain->GetNativeSwapChain();

	ComPtr<ID3D12Resource2> backbuffer;
	sc->GetBuffer(sc->GetCurrentBackBufferIndex(), IID_PPV_ARGS(&backbuffer));
	DirectX::SimpleMath::Color color = DirectX::Colors::LightBlue;



	list.SetScissorRects(mScissorRect);
	list.SetVieports(mScreenViewport);

	list.ClearRenderTarget(backbuffer, nullptr, color, 0, nullptr);
	
	auto rs = device.GetOrCreateRootSignature();
	list.SetRootSignature(*rs);

	std::vector<ComPtr<ID3D12Resource2>> res;
	res.push_back(backbuffer);

	list.SetRenderTargets(res);
	mesh->Draw(list);

	list.SetPresentState(backbuffer);

	list.Close();
	q.ExecuteCommandList(list);

	swapChain->Present();

	//DirectResource* myBuffer = nullptr;

}
