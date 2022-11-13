#include "Mouse.h"
#include <Windows.h>

Mouse::Event Mouse::GetEvent() noexcept
{
	if (_eventBuffer.size() > 0)
	{
		Mouse::Event e = _eventBuffer.front();
		_eventBuffer.pop();
		return e;
	}
	return Mouse::Event();
}

void Mouse::OnMouseMove(int x, int y) noexcept
{
	_x = x;
	_y = y;

	_eventBuffer.push(Event(Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	_leftIsPressed = true;
	_eventBuffer.push(Event(Event::Type::LPressed, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	_rightIsPressed = true;
	_eventBuffer.push(Event(Event::Type::RPressed, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	_leftIsPressed = false;
	_eventBuffer.push(Event(Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	_rightIsPressed = false;
	_eventBuffer.push(Event(Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	_eventBuffer.push(Event(Event::Type::WheelUP, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	_eventBuffer.push(Event(Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	_isInWindow = true;
	_eventBuffer.push(Event(Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	_isInWindow = false;
	_eventBuffer.push(Event(Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	_wheelDeltaCarry += delta;

	while (_wheelDeltaCarry >= WHEEL_DELTA)
	{
		_wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}

	while (_wheelDeltaCarry <= -WHEEL_DELTA)
	{
		_wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

void Mouse::TrimBuffer() noexcept
{
	while (_eventBuffer.size() > _bufferSize)
		_eventBuffer.pop();
}
