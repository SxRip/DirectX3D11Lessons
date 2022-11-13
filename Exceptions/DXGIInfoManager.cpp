#include "DXGIInfoManager.hpp"
#include "..\\Window\Window.h"
#include "..\\Graphics\Graphics.hpp"
#include <memory>

#pragma comment (lib, "dxguid.lib")

HRESULT _Hr_result;
#define GFX_THROW_NOINFO(returnHrCall) if(FAILED(_Hr_result = (returnHrCall))) throw Graphics::GFXException(__FILE__, __LINE__, _Hr_result)

DXGIInfoManager::DXGIInfoManager() : pDxgiInfoQueue{}, _Last_message{}
{
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);
	const auto pfnGetDebugInterface = dll.get_func<DXGIGetDebugInterface>("DXGIGetDebugInterface", "dxgidebug.dll");

	if (!pfnGetDebugInterface)
		throw WINEX_LAST_ERROR();
	GFX_THROW_NOINFO(pfnGetDebugInterface(__uuidof(IDXGIInfoQueue), &pDxgiInfoQueue));
}

DXGIInfoManager::~DXGIInfoManager()
{
	SafeRelease(pDxgiInfoQueue);
}

std::vector<std::string> DXGIInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const UINT64 end = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

	for (UINT64 i = _Last_message; i < end; ++i)
	{
		SIZE_T tmpMessageLenght = 0;

		GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessageA(DXGI_DEBUG_ALL, i, nullptr, &tmpMessageLenght));

		std::unique_ptr<byte[]> pBytes = std::make_unique<byte[]>(tmpMessageLenght);
		DXGI_INFO_QUEUE_MESSAGE* pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(pBytes.get());

		GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessageA(DXGI_DEBUG_ALL, i, pMessage, &tmpMessageLenght));
		messages.emplace_back(pMessage->pDescription);
	}

	return messages;
}

void DXGIInfoManager::SetMessageNum() noexcept
{
	_Last_message = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

