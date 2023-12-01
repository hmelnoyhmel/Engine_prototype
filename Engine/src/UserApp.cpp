#include "UserApp.h"

#include <iostream>

#include "DirectCommandQueue.h"
#include "GameWindow.h"
#include "DirectDevice.h"

UserApp::UserApp(HINSTANCE hInstance) :
	m_hAppInstance{ hInstance }
{

}

template <typename T, typename ... Args>
std::shared_ptr<T> UserApp::AddWindow(Args&& ... args) 
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
		if (msg == WM_DESTROY)
		{
			m_device->GetCommandQueue(EQueueType::Graphics).FlushCmdQueue();
			if (windowsMap.size() == 1)
			{
				PostQuitMessage(0);
			}
			windowsMap.erase(hWnd);
		}
	}
}

void UserApp::Init()
{
	m_device = new DirectDevice();
	m_device->CreateDevice();

	AddWindow<GameWindow>(*m_device, 800, 600);
	//AddWindow<GameWindow>(*m_device, 1200, 700);

	auto q = m_device->GetCommandQueue(EQueueType::Graphics);
	auto list = q.GetCommandList();


	list.GetNativeList()->Close();
	q.ExecuteCommandList(list);


}

void UserApp::Tick()
{
	// Add here subsystem updates

	for (auto& pair : windowsMap )
	{
		pair.second->Render();
	}

}

