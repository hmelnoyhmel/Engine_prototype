#include "GameWindow.h"

#include "DirectSwapChain.h"
#include "Event.h"

GameWindow::GameWindow(HINSTANCE hInstance, DirectDevice& device, unsigned int width, unsigned int height) :
	m_device{ device }
{
	m_hwnd = CreateWindow(
		L"BasicWndClass", // Registered WNDCLASS instance to use.
		m_windowName.c_str(), // window title
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
	CreateWindowSwapChain(m_device, width, height);
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);


}

LRESULT GameWindow::ProcessMessage(unsigned int msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		auto const pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		MessageBox(nullptr, L"Hello, World", L"Hello", MB_OK);
		return 0;
	}
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
			DestroyWindow(m_hwnd);
		return 0;
	}
	case WM_SIZE:
	{
		m_windowSwapChain->Resize(LOWORD(lParam), HIWORD(lParam));
		std::wstring const text = m_windowName +
			L"    width: " + std::to_wstring(GetWidth()) +
			L"    height: " + std::to_wstring(GetHeight());
		SetWindowText(m_hwnd, text.c_str());
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
	SetWindowText(m_hwnd, windowName.c_str());
}


void GameWindow::CreateWindowSwapChain(DirectDevice& device, int width, int height)
{
	m_windowSwapChain = std::make_shared<DirectSwapChain>(device, m_hwnd, width, height);
}

unsigned int GameWindow::GetWidth() const
{
	return m_windowSwapChain->GetWidth();
}

unsigned int GameWindow::GetHeight() const
{
	return m_windowSwapChain->GetHeight();
}

void GameWindow::Render()
{
	// Get cube
	// Draw cube
	// Call Present()
	// Update Fence

	// Causes memory leak
	m_windowSwapChain->Present();

}
