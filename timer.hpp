#pragma once
#include <chrono>

class _Simple_timer
{
public:
	_Simple_timer() : _point{std::chrono::steady_clock::now()} {}

	inline float mark() noexcept
	{
		const auto oldTime = _point;
		_point = std::chrono::steady_clock::now();
		const std::chrono::duration<float> frameTime = oldTime - _point;
		return frameTime.count();
	}

	inline float peek() const
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - _point).count();
	}

private:
	std::chrono::steady_clock::time_point _point;
};

