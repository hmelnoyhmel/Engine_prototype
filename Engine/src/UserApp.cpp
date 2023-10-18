#include "UserApp.h"
#include "Event.h"
#include "Win32.h"
#include "DirectDevice.h"

UserApp::UserApp(unsigned int width, unsigned int height, std::wstring windowName) :
	m_appWidth{ width }, m_appHeight{ height }, m_windowName{ windowName }
{

}

void UserApp::Init()
{
	auto callback = [this](ScreenResizeMessage msg) { this->SetResolution(msg); };
	Event<ScreenResizeMessage>::Subscribe(callback);

	pm_device = std::make_shared<DirectDevice>(Win32::GetHwnd());
	//pm_device->CreateCmdQueue();
}

void UserApp::SetResolution(ScreenResizeMessage msg)
{
	m_appWidth = msg.newWidth;
	m_appHeight = msg.newHeight;
}