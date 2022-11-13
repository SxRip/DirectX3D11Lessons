#pragma once
#include "..\\DllInterface.hpp"
#include <vector>
#include <string>
#include <dxgidebug.h>
#include <wrl.h>

class DXGIInfoManager
{
public:
	DXGIInfoManager();

	~DXGIInfoManager() = default;

	std::vector<std::string> GetMessages() const;
	void SetMessageNum() noexcept;
private:
	DLL dll;
	UINT64 _Last_message;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
};

