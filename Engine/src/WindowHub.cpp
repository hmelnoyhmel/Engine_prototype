#include "WindowHub.h"
#include "UserApp.h"

static UserApp* appGlobalInstance;

LRESULT CALLBACK WindowHub::HubWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	appGlobalInstance->AppWndProc(hWnd, msg, wParam, lParam);
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WindowHub::Run(UserApp& app)
{
	appGlobalInstance = &app;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = HubWndProc;
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
	long int frameCounter = 0;
	int x = 0;
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

		if (frameCounter%200 == 0)
		{
			x++;
		}

		app.Tick(x);
		frameCounter++;
	}

	return static_cast<int>(msg.wParam);
}