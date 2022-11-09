#include "App.h"

int App::run()
{
	while (true)
	{
		if (const auto& _Val = _window.ProcessMessages())
			return *_Val;
		DoFrame();
	}
	return 0;
}

void App::DoFrame()
{
	//const float c = sin(_timer.peek() / 2);
	_window.gfx().clearWindow(rn.get_double(), rn.get_double(), rn.get_double());
	_window.gfx().EndFrame();
}