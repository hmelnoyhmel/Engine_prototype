#pragma once
#include "DirectDevice.h"
#include <Windows.h>

class GameWindow;
class WindowsSystem;
class DirectDevice;

class UserApp
{
	friend WindowsSystem;
private:
	HINSTANCE m_hAppInstance;
	DirectDevice* m_device; // temp

	std::unordered_map<HWND, std::shared_ptr<GameWindow>> windowsMap;
	void WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	UserApp(HINSTANCE hInstance);
	UserApp() = default;
	virtual void Init();
	virtual void Tick();

	inline HINSTANCE GetInstance() const { return m_hAppInstance; }

	template <typename T, typename ... Args>
	std::shared_ptr<T> AddWindow(Args&& ... args);
};