#include "UserApp.h"
#include "DirectDevice.h"
#include "GameWindow.h"
#include "DirectCommandQueue.h"

UserApp::UserApp(HINSTANCE hInstance) :
	appInstance{hInstance}
{
	device = new DirectDevice();
	device->CreateDevice();
}

template <typename T, typename... Args>
std::shared_ptr<T> UserApp::AddWindow(Args&&... args)
{
	static_assert(std::is_base_of_v<GameWindow, T>, "T must inherit from GameWindow");
	auto window = std::make_shared<GameWindow>(appInstance, std::forward<Args>(args)...);
	windows[window->GetHWND()] = window;
	return window;
}


void UserApp::AppWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto const mapIterator = windows.find(hwnd);
	if (mapIterator != windows.end())
	{
		auto const window = mapIterator->second;
		window->ProcessMessage(msg, wParam, lParam);
		if (msg == WM_DESTROY)
		{
			device->GetCommandQueue(EQueueType::Graphics).FlushCmdQueue();
			if (windows.size() == 1)
			{
				PostQuitMessage(0);
			}
			windows.erase(hwnd);
		}
	}
}

void UserApp::Init()
{
	AddWindow<GameWindow>(*device, 800, 600);
	// AddWindow<GameWindow>(*device, 1200, 600);

	for (auto& pair : windows)
	{
		Vertex triangleVertices[] =
		{
			{{0.0f, 0.25f * pair.second->GetAspectRatio(), 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
			{{0.25f, -0.25f * pair.second->GetAspectRatio(), 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
			{{-0.25f, -0.25f * pair.second->GetAspectRatio(), 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}
		};
	}
}

void UserApp::Tick()
{
	for (const auto& pair : windows)
	{
		pair.second->Render();
	}
}
