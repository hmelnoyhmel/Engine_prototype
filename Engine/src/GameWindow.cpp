#include "GameWindow.h"
#include "Event.h"

GameWindow::GameWindow(HINSTANCE hInstance, unsigned int width, unsigned int height) :
	m_windowWidth{ width }, m_windowHeight{ height }
{
	m_hwnd = CreateWindow(
		L"BasicWndClass", // Registered WNDCLASS instance to use.
		m_windowName.c_str(), // window title
		WS_OVERLAPPEDWINDOW, // style flags
		CW_USEDEFAULT, // x-coordinate
		CW_USEDEFAULT, // y-coordinate
		m_windowWidth, // width
		m_windowHeight, // height
		nullptr, // parent window
		nullptr, // menu handle
		hInstance, // app instance
		nullptr); // extra creation parameters

	SetWindowName(L"TestWindow");
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
		int const mClientWidth = LOWORD(lParam);
		int const mClientHeight = HIWORD(lParam);
		ScreenResizeMessage message = { message.newWidth = mClientWidth, message.newHeight = mClientHeight };
		Event<ScreenResizeMessage>::Raise(message);
		std::wstring const text =
			L"    width: " + std::to_wstring(m_windowWidth) +
			L"    height: " + std::to_wstring(m_windowHeight);
		SetWindowText(m_hwnd, text.c_str());
		return 0;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
}

void GameWindow::SetWindowName(std::wstring windowName) const
{
	SetWindowText(m_hwnd, windowName.c_str());
}
