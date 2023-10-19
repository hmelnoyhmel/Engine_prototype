#include "Win32.h"
#include "Event.h"
#include "GameWindow.h"
#include "UserApp.h"

LRESULT CALLBACK Win32::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//std::shared_ptr<GameWindow> pWindow;
	auto const iteratorWindow = windowsMap.find(hWnd);
	if (iteratorWindow != windowsMap.end())
	{
		std::shared_ptr<GameWindow> const pWindow = iteratorWindow->second;
		pWindow->ProcessMessage(msg, wParam, lParam);
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
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
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

	windowsMap[m_hwnd] = std::make_shared<GameWindow>(m_hwnd, pApp->GetAppWidth(), pApp->GetAppHeight());

	pApp->Init();

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	MSG msg = { m_hwnd };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else // this part is the main game loop
		{
			CalculateFrameStats(pApp);
			// pApp->GameLoop(); ???
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