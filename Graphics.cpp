#include "Graphics.hpp"

HRESULT _Hr_return_code;
#define GFX_THROW_FAILED(returnHrFunc) if(FAILED(_Hr_return_code = returnHrFunc)){throw Graphics::GFXException(__FILE__, __LINE__, _Hr_return_code);}
#define GFX_DEVICE_REMOVED(hr) Graphics::RemovedDeviceEx(__FILE__, __LINE__, hr)

Graphics::Graphics(HWND hwnd) : pDevice{}, pCTX{}, pSwapChain{}, pTarget{}
{
	DXGI_SWAP_CHAIN_DESC sd{ sizeof(sd) };

	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = true;

	GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		0, nullptr, 0, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, nullptr, &pCTX));

	pTarget = _Create_render_target(pSwapChain);
}

void Graphics::EndFrame()
{
	HRESULT hr;
	if (FAILED(hr = pSwapChain->Present(2, 0)))
		if (hr == ERROR_DEVICE_REMOVED)
			throw GFX_DEVICE_REMOVED(pDevice->GetDeviceRemovedReason());
		else
			GFX_THROW_FAILED(hr);
}

ID3D11RenderTargetView* Graphics::_Create_render_target(IDXGISwapChain* _Swap_chain)
{
	ID3D11Resource* pBackBuffer = nullptr;
	GFX_THROW_FAILED(pSwapChain->GetBuffer(DXGI_SWAP_EFFECT_DISCARD,
		__uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)));

	ID3D11RenderTargetView* pRenderTarget = nullptr;
	GFX_THROW_FAILED(pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTarget));

	_SafeRelease(pBackBuffer);
	return pRenderTarget;
}

inline const char* Graphics::GFXException::what() const noexcept
{
	std::stringstream ss;
	ss << "[FILE] " << _file << std::endl
		<< "[LINE] " << _line << std::endl
		<< "[DX ERROR STRING] " << get_error_string() << std::endl
		<< "[DX ERROR DESCRIPTION] " << get_error_description() << std::endl;
	_error = ss.str();
	return _error.c_str();
}

inline const char* Graphics::GFXException::get_type() const noexcept
{
	return "[GFX Exception]";
}

inline std::string Graphics::GFXException::get_error_description() const noexcept
{
	char cBuff[512]{};
	DXGetErrorDescription(_hr, cBuff, sizeof(cBuff));
	return cBuff;
}

inline const char* Graphics::RemovedDeviceEx::what() const noexcept
{
	std::stringstream ss;
	ss << "[FILE] " << _file << std::endl
		<< "[LINE] " << _line << std::endl
		<< "[CODE] " << _hr << std::endl;

	_error = ss.str();
	return _error.c_str();
}
