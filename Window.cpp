#include "Window.h"

Window::WindowClass::WindowClass()
	: hInst{ GetModuleHandle(nullptr) }
{
	WNDCLASSEX wc{ sizeof(wc) };

	wc.hCursor = LoadCursor(hInst, IDC_ARROW);
	wc.lpszClassName = wndClassName;
	wc.hInstance = hInst;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.hIcon = static_cast<HICON>(LoadImage(wc.hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wc.hIconSm = static_cast<HICON>(LoadImage(wc.hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));


	if (!RegisterClassEx(&wc))
		if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
			throw WINEX_LAST_ERROR();
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, hInst);
}

LRESULT CALLBACK Window::HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCT* pCreateS =
			reinterpret_cast<CREATESTRUCT*>(lParam);

		Window* const pWnd = static_cast<Window*>(pCreateS->lpCreateParams);

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageThunk));

		pWnd->WndProc(hwnd, msg, wParam, lParam);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMessageThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	return pWnd->WndProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_KILLFOCUS:
		keyboard.ClearState();
		break;

	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < _width && pt.y >= 0 && pt.y < _height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.InWindow())
			{
				SetCapture(_hwnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
				mouse.OnMouseMove(pt.x, pt.y);
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}

	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}

	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}

	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}

	case WM_MOUSEWHEEL:
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnWheelDelta(pt.x, pt.y, GET_WHEEL_DELTA_WPARAM(wParam));

		break;

	case WM_KEYDOWN:

	case WM_SYSKEYDOWN:
		if (!(lParam & KF_REPEAT) || keyboard.AutoRepeatEnabled())
			keyboard.OnKeyPressed(static_cast<UCHAR>(wParam));
		break;

	case WM_KEYUP:
		keyboard.OnKeyReleased(static_cast<UCHAR>(wParam));
		break;

	case WM_CHAR:
		keyboard.OnChar(static_cast<UCHAR>(wParam));
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return msg.wParam;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

Window::Window(size_t widht, size_t height, const char* wndName)
	: _width{ widht }, _height{ height }
{
	RECT rc{ 0,0, widht, height };

	if (!AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false))
		throw WINEX_LAST_ERROR();

	int centerX = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
	int centerY = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

	_hwnd = CreateWindowEx(WS_EX_TOPMOST, _wndClass.GetWndClass(), wndName, WS_OVERLAPPEDWINDOW,
		centerX, centerY, rc.right - rc.left, rc.bottom - rc.top, nullptr,
		nullptr, _wndClass.GetHinstance(), this);

	if (!_hwnd)
		throw WINEX_LAST_ERROR();

	ShowWindow(_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(_hwnd);
}

Window::~Window()
{
	DestroyWindow(_hwnd);
}

void Window::set_title(const char* title) const noexcept
{
	if (!SetWindowText(_hwnd, title))
		throw WINEX_LAST_ERROR();
}

const char* Window::WindowException::what() const noexcept
{
	std::stringstream ss;

	ss << "[ERROR CODE] " << _hr << std::endl
		<< "[FILE] " << _file << std::endl
		<< "[LINE] " << _line << std::endl
		<< std::endl << _get_error_msg() << std::endl;

	_error = ss.str();
	return _error.c_str();
}

std::string Window::WindowException::_get_error_msg() const noexcept
{
	char* pErrorMsg = nullptr;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, _hr, LANG_SYSTEM_DEFAULT,
		reinterpret_cast<LPSTR>(&pErrorMsg), 0, 0);

	if (!pErrorMsg)
		return "Unknow error";

	std::string error = pErrorMsg;
	LocalFree(pErrorMsg);

	return error;
}
