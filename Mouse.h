#pragma once
#include <queue>

class Mouse
{
	using UINT = unsigned int;
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			RPressed,
			LPressed,
			RRelease,
			LRelease,
			WheelUP,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};

		Event()
			: _type{ Type::Invalid }, _leftIsPressed{}, _rightIsPressed{}, _x{}, _y{}
		{}

		Event(Type type, const Mouse& parent)
			: _type{ type }, _leftIsPressed{ parent._leftIsPressed },
			_rightIsPressed{ parent._rightIsPressed }, _x{ parent._x }, _y{ parent._y }
		{}

		inline Type GetType() const noexcept { return _type; }

		inline bool IsValid() const noexcept { return _type != Type::Invalid; }
		inline bool LeftIsPressed() const noexcept { return _leftIsPressed; }
		inline bool RigthIsPressed() const noexcept { return _rightIsPressed; }

		inline int GetX() const noexcept { return _x; }
		inline int GetY() const noexcept { return _y; }

		std::pair<int, int> GetPos() const noexcept { return { _x, _y }; }

	private:
		Type _type;
		bool _leftIsPressed;
		bool _rightIsPressed;
		int _x, _y;
	};

	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&&) = delete;

	inline std::pair<int, int> GetPos() const noexcept { return { _x, _y }; }
	inline int GetPosX() const noexcept { return _x; }
	inline int GetPosY() const noexcept { return _y; }

	inline bool LeftIsPressed() const noexcept { return _leftIsPressed; }
	inline bool RightIsPressed() const noexcept { return _rightIsPressed; }
	inline bool InWindow() const noexcept { return _isInWindow; }

	inline bool IsEmpty() const noexcept { return _eventBuffer.empty(); }
	inline void Flush() noexcept { _eventBuffer = std::queue<Event>(); }

	Event GetEvent() noexcept;

private:
	void OnMouseMove(int x, int y) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseLeave() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
	void TrimBuffer() noexcept;

	const UINT _bufferSize = 16;

	std::queue<Event> _eventBuffer;

	bool _leftIsPressed, _rightIsPressed, _isInWindow;
	int _x, _y, _wheelDeltaCarry = 0;
};

