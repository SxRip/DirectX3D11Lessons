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

}
