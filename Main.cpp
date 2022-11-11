#include "App.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PCHAR Args, int nCmdShow)
{
	try
	{
		return App{}.run();
	}
	catch (const BaseException& ex)
	{
		MessageBox(nullptr, ex.what(), ex.get_type(), MB_ICONERROR);
	}
	catch (...)
	{
		MessageBox(nullptr, "Something wrong", nullptr, MB_ICONERROR);
	}
}