#pragma once
#include <bitset>
#include <queue>

class Keyboard
{
	friend class Window;
	using UINT = unsigned int;
	using UCHAR = unsigned char;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Relese,
			Invalid
		};
		Event() : _type{ Type::Invalid }, _code{}
		{}
		Event(Type _type, UCHAR code) : _type{ _type }, _code{ code }
		{}

		inline bool IsPress() const noexcept { return _type == Type::Press; }
		inline bool IsRelease() const noexcept { return _type == Type::Relese; }
		inline bool IsValid() const noexcept { return _type != Type::Invalid; }

		inline UCHAR GetCode() const noexcept { return _code; }
	private:
		UCHAR _code;
		Type _type;
	};

	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&&) = delete;

	inline bool KeyIsPressed(UCHAR code) const noexcept { return _keyStates[code]; }

	Event ReadKey() noexcept;
	inline bool KeyIsEmpty() const noexcept { return _keyEventBuffer.empty(); };
	inline void FlushKeyStateBuffer() noexcept { _keyStates = std::bitset<256>(); };

	char ReadChar() noexcept;
	inline bool CharIsEmpty() const noexcept { return _keyCharBuffer.empty(); };
	inline void FlushCharBuffer() noexcept { _keyCharBuffer = std::queue<char>(); };

	void FlushAllBuffers() noexcept;

	inline void EnableAutoRepeat() noexcept { _autoRepeatEnabled = true; };
	inline void DisableAutoRepeat() noexcept { _autoRepeatEnabled = false; };
	inline bool AutoRepeatEnabled() const noexcept { return _autoRepeatEnabled; }

private:
	void OnKeyPressed(UCHAR code) noexcept;
	void OnKeyReleased(UCHAR code) noexcept;
	void OnChar(UCHAR code) noexcept;

	inline void ClearState() noexcept { _keyStates.reset(); };

	template<class _Ty>
	void TrimBuffer(std::queue<_Ty>& buffer) noexcept;

	const UINT _bufferSize = 16;

	bool _autoRepeatEnabled = false;

	std::bitset<256u> _keyStates;
	std::queue<Event> _keyEventBuffer;
	std::queue<char> _keyCharBuffer;
};

template<class _Ty>
inline void Keyboard::TrimBuffer(std::queue<_Ty>& buffer) noexcept
{
	while (buffer.size() > _bufferSize)
		buffer.pop();
}