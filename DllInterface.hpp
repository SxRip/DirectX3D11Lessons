#pragma once
#include <Windows.h>
#include <map>
#include <string>

void SafeFreeLibrary(HMODULE& hModule);

typedef std::pair<const char*, HMODULE> pairCHAR_MODULE;

class DLL
{
public:
	DLL();
	~DLL();

	template <class _Ty>
	_Ty get_func(const char* functionName, const char* dllName) noexcept;

	template <class _Ty>
	inline _Ty get_func(const char* functionName) const noexcept;

	void pop_front() noexcept;
	void pop_back() noexcept;
	void clear() noexcept;

	inline size_t size() const noexcept { return _modules.size(); };

	inline void push_back(const char* dllName, HMODULE dllInfo) noexcept { _modules.emplace(dllName, dllInfo); };
	bool push_back(const char* dllName) noexcept;
	void push_back(HMODULE dllInfo) noexcept;

private:
	std::map<const char*, HMODULE> _modules;
};

template<class _Ty>
_Ty DLL::get_func(const char* functionName, const char* dllName) noexcept
{
	if (_modules.size() > 0)
	{
		const auto it = _modules.find(dllName);

		if (it != _modules.end())
			return reinterpret_cast<_Ty>(GetProcAddress(it->second, functionName));
	}

	HMODULE hDLL = LoadLibrary(dllName);

	if (!hDLL)
		return nullptr;

	_modules.emplace(dllName, hDLL);

	return reinterpret_cast<_Ty>(GetProcAddress(hDLL, functionName));
}

template<class _Ty>
inline _Ty DLL::get_func(const char* functionName) const noexcept
{
	for (const auto& elem : _modules)
	{
		FARPROC func = GetProcAddress(elem.second, functionName);

		if (func)
			return reinterpret_cast<_Ty>(func);
	}

	return nullptr;
}
