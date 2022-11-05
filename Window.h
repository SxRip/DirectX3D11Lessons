#pragma once
#include <Windows.h>
#include <exception>
#include "BaseException.hpp"
#include "resource.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.hpp"
#include <optional>
#include <memory>

class Window
{
	class WindowClass
	{
	public:
		WindowClass();
		~WindowClass();

		WindowClass(const WindowClass&) noexcept = delete;
		WindowClass operator=(const WindowClass&) noexcept = delete;

		inline const char* GetWndClass() const noexcept { return wndClassName; }
		inline HINSTANCE GetHinstance() const noexcept { return hInst; };
	private:
		const char* wndClassName = "D3D Love";
		HINSTANCE hInst;
	};

	static LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMessageThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


public:

	class WindowException : public BaseException
	{
	public:
		WindowException(const char* file, size_t line, HRESULT hr)
			: BaseException(file, line, hr)
		{}

		virtual const char* what() const noexcept;
		virtual const char* get_type() const noexcept { return "Window Exception"; }

	private:
		std::string _get_error_msg() const noexcept;
	};


	Window(size_t widht, size_t height, const char* wndName);
	~Window();

	Window(const Window&) noexcept = delete;
	Window operator=(const Window&) noexcept = delete;

	void set_title(const char* title) const noexcept;
	std::optional<int> ProcessMessages();

	Keyboard keyboard;
	Mouse mouse;

	inline Graphics& gfx() const noexcept { return *pGFX; }

private:
	std::unique_ptr<Graphics> pGFX;

	size_t _width, _height;
	HWND _hwnd;
	WindowClass _wndClass;
};

#define WINEX_LAST_ERROR() Window::WindowException(__FILE__, __LINE__, GetLastError())
#define WINEX_ERROR(hr) Window::WindowException(__FILE__, __LINE__, hr)
