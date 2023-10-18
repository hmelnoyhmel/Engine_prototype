#include "Win32.h"
#include "Event.h"
#include "UserApp.h"

LRESULT CALLBACK Win32::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	UserApp* pApp = reinterpret_cast<UserApp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (msg)
	{
	case WM_CREATE:
		{
			LPCREATESTRUCT const pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
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
				DestroyWindow(hWnd);
			return 0;
		}

	case WM_SIZE:
		{
			int const mClientWidth = LOWORD(lParam);
			int const mClientHeight = HIWORD(lParam);
			ScreenResizeMessage message = { message.newWidth = mClientWidth, message.newHeight = mClientHeight };
			Event<ScreenResizeMessage>::Raise(message);
			return 0;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int Win32::Run(UserApp* pApp, HINSTANCE hInstance, int nCmdShow)
{

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = L"BasicWndClass";


	if (!RegisterClass(&wc))
	{
		MessageBox(nullptr, L"RegisterClass FAILED", nullptr, 0);
		return false;
	}


	m_hwnd = CreateWindow(
		L"BasicWndClass", // Registered WNDCLASS instance to use.
		pApp->GetAppWindowName().c_str(), // window title
		WS_OVERLAPPEDWINDOW, // style flags
		CW_USEDEFAULT, // x-coordinate
		CW_USEDEFAULT, // y-coordinate
		pApp->GetAppWidth(), // width
		pApp->GetAppHeight(), // height
		nullptr, // parent window
		nullptr, // menu handle
		hInstance, // app instance
		nullptr); // extra creation parameters

	if (m_hwnd == nullptr)
	{
		MessageBox(nullptr, L"CreateWindow FAILED", nullptr, 0);
		return false;
	}

	pApp->Init();

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	MSG msg = { nullptr };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // this part is the main game loop
		{
			CalculateFrameStats(pApp);
		}
	}
	return static_cast<int>(msg.wParam);
}


void Win32::CalculateFrameStats(UserApp* pApp)
{
	std::wstring const w = std::to_wstring(pApp->GetAppWidth());
	std::wstring const h = std::to_wstring(pApp->GetAppHeight());
	std::wstring const mainWndCaption(pApp->GetAppWindowName());

	std::wstring const text = mainWndCaption +
		L"    width: " + w +
		L"    height: " + h;

	SetWindowText(m_hwnd, text.c_str());
}