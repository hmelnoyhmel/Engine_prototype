#pragma once
#include <string>
#include <memory>

class Win32;
struct ScreenResizeMessage;
class DirectDevice;

class UserApp
{
public:

	UserApp(unsigned int width, unsigned int height, std::wstring windowName);

	void Init();

	inline unsigned int GetAppWidth() const { return m_appWidth; }
	inline unsigned int GetAppHeight() const { return m_appHeight; }
	inline std::wstring GetAppWindowName() const { return m_windowName; }
	void SetResolution(ScreenResizeMessage msg);

private:
	unsigned int m_appWidth;
	unsigned int m_appHeight;
	std::wstring m_windowName;

	std::shared_ptr<DirectDevice> pm_device;
};
