#include "Graphics.hpp"

HRESULT _Hr_return_code;
#define GFX_EXPECT_NOINFO(hr) Graphics::GFXException(__FILE__, __LINE__, hr);

#define GFX_THROW_NOINFO(returnHrCall) if(FAILED(_Hr_return_code = (returnHrCall))) throw Graphics::GFXException(__FILE__, __LINE__, _Hr_return_code)

#ifdef _DEBUG
#define GFX_EXCEPT(hr) Graphics::GFXException(__FILE__, __LINE__, hr, _DXGIInfo.GetMessages());
#define GFX_THROW_INFO(returnHrFunc) _DXGIInfo.SetMessageNum(); if(FAILED(_Hr_return_code = returnHrFunc)) throw Graphics::GFXException(__FILE__, __LINE__, _Hr_return_code, _DXGIInfo.GetMessages())
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::RemovedDeviceEx(__FILE__, __LINE__, hr, _DXGIInfo.GetMessages())
#define GFX_THROW_VOID_INFO(returnHrFunc) _DXGIInfo.SetMessageNum(); (returnHrFunc); std::vector<std::string> msgs = _DXGIInfo.GetMessages(); if(!msgs.empty()) throw GFXException(__FILE__, __LINE__, msgs)
#else

#define GFX_EXCEPT(hr) Graphics::GFXException(__FILE__, __LINE__, hr)
#define GFX_THROW_INFO(returnHrFunc) GFX_THROW_NOINFO(returnHrFunc)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::RemovedDeviceEx(__FILE__, __LINE__, hr)
#define GFX_THROW_VOID_INFO(returnHrFunc) (returnHrFunc)
#endif


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
	UINT swapCreateFlags = 0;

#ifdef _DEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		swapCreateFlags, nullptr, 0, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, nullptr, &pCTX));

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	GFX_THROW_INFO(pSwapChain->GetBuffer(DXGI_SWAP_EFFECT_DISCARD,
		__uuidof(ID3D11Resource), &pBackBuffer));

	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));
}

void Graphics::DrawRectangle()
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC BufferDesc{ sizeof(BufferDesc) };
	D3D11_SUBRESOURCE_DATA SubResData{};

	struct Vertex
	{
		float x, y;
	};

	const Vertex vertices[] =
	{
		{0.f, 0.5f},
		{0.5f, -0.5f},
		{-0.5f, -0.5f}
	};


	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.StructureByteStride = sizeof(Vertex);
	BufferDesc.ByteWidth = sizeof(BufferDesc);
	SubResData.pSysMem = vertices;

	const UINT stride = sizeof(Vertex);
	const UINT offsets = 0u;

	GFX_THROW_INFO(pDevice->CreateBuffer(&BufferDesc, &SubResData, &pVertexBuffer));

	pCTX->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offsets);
	GFX_THROW_VOID_INFO(pCTX->Draw(3u, 0u));
}

void Graphics::EndFrame()
{
	HRESULT hr;

#ifdef _DEBUG
	_DXGIInfo.SetMessageNum();
#endif // _DEBUG

	if (FAILED(hr = pSwapChain->Present(2, 0)))
		if (hr == ERROR_DEVICE_REMOVED)
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		else
			throw GFX_EXCEPT(hr);
}

inline const char* Graphics::GFXException::what() const noexcept
{
	std::stringstream ss;
	ss << "[FILE] " << _file << std::endl
		<< "[LINE] " << _line << std::endl
		<< "[ERROR CODE] " << "0x" << std::hex << std::uppercase << _hr
		<< std::dec << " ( " << static_cast<size_t>(_hr) << " ) " << std::endl
		<< "[DX ERROR STRING] " << get_error_string() << std::endl
		<< "[DX ERROR DESCRIPTION] " << get_error_description() << std::endl;

	if (!_info.empty())
		ss << "\n[ERROR INFO]\n" << _info << std::endl;

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
