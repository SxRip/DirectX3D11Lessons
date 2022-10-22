#include <Windows.h>
#include "Window.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PCHAR Args, int nCmdShow)
{
	int delta = 0;
	try
	{
		Window wnd(600, 400, "Window");
		MSG msg{};
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			DispatchMessage(&msg);
			TranslateMessage(&msg);

			const Mouse::Event e = wnd.mouse.GetEvent();

			switch (e.GetType())
			{
				/*case Mouse::Event::Type::Enter:
				{
					wnd.set_title("Enter");
					break;
				}
				case Mouse::Event::Type::Leave:
					wnd.set_title("Gone");
					break;

				case Mouse::Event::Type::Move:
				{
					std::stringstream ss;
					ss << "X:" << e.GetX() << " Y: " << e.GetY();
					wnd.set_title(ss.str().c_str());
					break;
				}*/

				case Mouse::Event::Type::WheelUP:
				{
					wnd.set_title(std::to_string(++delta).c_str());
					break;
				}

				case Mouse::Event::Type::WheelDown:
				{
					wnd.set_title(std::to_string(--delta).c_str());
					break;
				}

			}

		}
	}
	catch (const BaseException& ex)
	{
		MessageBox(nullptr, ex.what(), ex.get_type(), MB_ICONERROR);
	}
	catch (...)
	{
		MessageBox(nullptr, "Something wrong", nullptr, MB_ICONERROR);
	}
	return 0;
}