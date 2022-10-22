#pragma once
#include "Window.h"

class App
{
public:
	App() : _window(400, 300, "DirectX 3D") {}

	int run();

private:
	void DoFrame();
	Window _window;
};

