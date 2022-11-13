#include "Keyboard.h"

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (_keyEventBuffer.size() > 0)
	{
		Keyboard::Event e = _keyEventBuffer.front();
		_keyEventBuffer.pop();
		return e;
	}
	return Keyboard::Event();
}

char Keyboard::ReadChar() noexcept
{
	if (_keyCharBuffer.size() > 0)
	{
		UCHAR e = _keyCharBuffer.front();
		_keyCharBuffer.pop();
		return e;
	}
	return 0;
}

void Keyboard::FlushAllBuffers() noexcept
{
	FlushCharBuffer();
	FlushKeyStateBuffer();
}

void Keyboard::OnKeyPressed(UCHAR code) noexcept
{
	_keyStates[code] = true;
	_keyEventBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, code));
	TrimBuffer(_keyEventBuffer);
}

void Keyboard::OnKeyReleased(UCHAR code) noexcept
{
	_keyStates[code] = false;
	_keyEventBuffer.push(Keyboard::Event(Keyboard::Event::Type::Relese, code));
	TrimBuffer(_keyEventBuffer);
}

void Keyboard::OnChar(UCHAR code) noexcept
{
	_keyCharBuffer.push(code);
	TrimBuffer(_keyCharBuffer);
}