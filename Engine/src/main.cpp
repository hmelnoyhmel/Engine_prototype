#include <Windows.h>
#include <iostream>
#include "UserApp.h"
#include "WindowHub.h"

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR pCmdLine, int nShowCmd)
{
	UserApp app(hInstance);
	try
	{
		WindowHub::Run(app);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}