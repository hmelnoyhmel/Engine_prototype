#include <Windows.h>
#include <iostream>
#include <stdexcept>
#include "UserApp.h"
#include "Win32.h"

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR pCmdLine, int nShowCmd)
{
	UserApp appSample(800, 600, L"Simple Window, nothing else");

	try
	{
		Win32::Run(&appSample, hInstance, nShowCmd);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}