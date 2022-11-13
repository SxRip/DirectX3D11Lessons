#pragma once
#include <Windows.h>
#include <exception>
#include "..\\Exceptions\BaseException.hpp"
#include "..\\Resource.h"
#include "..\\Keyboard\\Keyboard.h"
#include "..\\Mouse\Mouse.h"
#include "..\\Graphics\Graphics.hpp"
#include <optional>
#include <memory>

class Window
{
	class WindowClass
	{
	public:
		WindowClass();
		~WindowClass();

		WindowClass(const WindowClass&)				noexcept = delete;
		WindowClass operator=(const WindowClass&)	noexcept = delete;

		inline const char* GetWndClass()	const noexcept { return wndClassName; }
		inline HINSTANCE	GetHinstance()	const noexcept { return hInst; };

	private:
		const char* wndClassName = "D3D Love";
		HINSTANCE	hInst;
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

		virtual const char* what()				const noexcept;
		virtual const char* get_type()			const noexcept { return "[Window exception]"; }

		std::string					get_error_msg()	const noexcept;
	};

	class NoGfxException : public BaseException
	{
	public:
		NoGfxException(const char* file, size_t line, HRESULT hr)
			: BaseException(file, line, hr)
		{}

		inline virtual const char* what()		const noexcept;
		inline virtual const char* get_type()	const noexcept { return "[Graphics object isn't initialized]"; }
	};

	Window(size_t widht, size_t height, const char* wndName);
	~Window();

	Window(const Window&)			noexcept = delete;
	Window operator=(const Window&) noexcept = delete;

	void set_title(const char* title) const;
	std::optional<int> ProcessMessages();

	Keyboard keyboard;
	Mouse mouse;

	Graphics& gfx() const;

private:
	std::unique_ptr<Graphics> pGFX;

	size_t _width, _height;
	HWND _hwnd;
	WindowClass _wndClass;
};

#define WINEX_LAST_ERROR()	Window::WindowException(__FILE__, __LINE__, GetLastError())
#define WINEX_ERROR(hr)		Window::WindowException(__FILE__, __LINE__, hr)
#define NOGFX_EXCEPT()		Window::NoGfxException(__FILE__, __LINE__, 0)