#include "DllInterface.hpp"

void SafeFreeLibrary(HMODULE& hModule)
{
	if (hModule != nullptr)
	{
		FreeLibrary(hModule);

		hModule = nullptr;
	}
}

DLL::DLL() :
	_modules{}
{}

DLL::~DLL()
{
	clear();
}

void DLL::pop_front() noexcept
{
	if (_modules.size() > 0)
	{
		SafeFreeLibrary(_modules.begin()->second);
		_modules.erase(_modules.begin()->first);
	}
}

void DLL::pop_back() noexcept
{
	if (_modules.size() > 0)
	{
		auto it = _modules.end(); --it;

		SafeFreeLibrary(it->second);
		_modules.erase(it);
	}
}

void DLL::clear() noexcept
{
	for (auto& elem : _modules)
		SafeFreeLibrary(elem.second);
	_modules.clear();
}

bool DLL::push_back(const char* dllName) noexcept
{
	HMODULE hDLL = LoadLibrary(dllName);

	if (!hDLL)
		return false;

	_modules.emplace(dllName, hDLL);
	return true;
}

void DLL::push_back(HMODULE dllInfo) noexcept
{
	char cdllPath[MAX_PATH]{};
	GetModuleFileName(dllInfo, cdllPath, sizeof(cdllPath));

	std::string dllPath = cdllPath;
	dllPath = dllPath.substr(dllPath.find_last_of("\\") + 1);

	_modules.emplace(dllPath.c_str(), dllInfo);
}
