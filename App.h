#pragma once
#include "random.hpp"
#include "Window.h"
#include "timer.hpp"

class App
{
public:
	App() : _window(400, 300, "DirectX 3D") { rn.set_range(0.f, 1.f); }

	int run();

private:
	void DoFrame();
	Window			_window;
	_Simple_timer	_timer;
	random			rn;
};

