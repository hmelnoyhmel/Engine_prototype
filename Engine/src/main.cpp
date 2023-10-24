#include <Windows.h>
#include <iostream>
#include <stdexcept>
#include "UserApp.h"
#include "WindowsSystem.h"

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR pCmdLine, int nShowCmd)
{
	UserApp appSample(hInstance);
	try
	{
		WindowsSystem::Run(appSample);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}