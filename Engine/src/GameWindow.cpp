#include "GameWindow.h"
#include "Event.h"

GameWindow::GameWindow(HWND hwnd, unsigned int width, unsigned int height) :
	m_hwnd{hwnd}, m_windowWidth{width}, m_windowHeight{height}
{
	
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
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	//return DefWindowProc(m_hwnd, msg, wParam, lParam);
}
