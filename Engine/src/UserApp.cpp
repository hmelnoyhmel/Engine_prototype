#include "UserApp.h"
#include "GameWindow.h"
#include "DirectDevice.h"

UserApp::UserApp(HINSTANCE hInstance) :
	m_hAppInstance{ hInstance }
{

}

template <typename T, typename ... Args>
std::shared_ptr<T> UserApp::AddWindow(Args&& ... args) // return type was T*
{
	static_assert(std::is_base_of_v<GameWindow, T>, "T must inherit from NewGameWindow");
	auto wndPtr = std::make_shared<GameWindow>(m_hAppInstance, std::forward<Args>(args)...);
	windowsMap[wndPtr->GetHWND()] = wndPtr;
	return wndPtr;
}


void UserApp::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto const iteratorWindow = windowsMap.find(hWnd);
	if (iteratorWindow != windowsMap.end())
	{
		auto const pWindow = iteratorWindow->second;
		pWindow->ProcessMessage(msg, wParam, lParam);
	}
}

void UserApp::Init()
{
	AddWindow<GameWindow>(800, 600);
	m_device = new DirectDevice();
}

void UserApp::Tick()
{

}

