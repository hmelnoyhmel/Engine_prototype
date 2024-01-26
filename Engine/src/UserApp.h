#pragma once
#include <DirectXMath.h>
#include <memory>
#include <unordered_map>
#include <Windows.h>

class GameWindow;
class WindowHub;
class DirectDevice;

class UserApp
{
	friend WindowHub;

public:
	UserApp(HINSTANCE hInstance);
	UserApp() = default;

	template <typename T, typename ... Args>
	std::shared_ptr<T> AddWindow(Args&& ... args);

	virtual void Init();
	virtual void Tick();

	inline HINSTANCE GetInstance() const { return appInstance; }

private:
	HINSTANCE appInstance;
	DirectDevice* device;

	std::unordered_map<HWND, std::shared_ptr<GameWindow>> windows;

	void AppWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	// 
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

};