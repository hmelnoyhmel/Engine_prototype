#include "WindowsSystem.h"
#include "UserApp.h"

static UserApp* instance;

LRESULT CALLBACK WindowsSystem::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	instance->WndProc(hWnd, msg, wParam, lParam);
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WindowsSystem::Run(UserApp& app)
{
	instance = &app;


	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = app.GetInstance();
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

	app.Init();

	MSG msg = { };

	bool run = true;
	while (run)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				run = false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		app.Tick();
	}

	return static_cast<int>(msg.wParam);
}