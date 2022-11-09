#pragma once
#include "BaseException.hpp"
#include "dxerr.h"
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

class Graphics
{
public:
	Graphics(HWND hwnd) : pDevice{}, pCTX{}, pSwapChain{}, pTarget{}
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

		HRESULT ok = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			0, nullptr, 0, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, nullptr, &pCTX);

		if (ok != S_OK || !pSwapChain || !pDevice || !pCTX)
			throw "D3D initialization error";
		
		pTarget = _Create_render_target(pSwapChain);
		if (!pTarget)
			throw "D3D initalization render target error";
	}

	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;


	~Graphics()
	{
		_SafeRelease(pDevice);
		_SafeRelease(pCTX);
		_SafeRelease(pSwapChain);
		_SafeRelease(pTarget);
	}

	void clearWindow(float r, float g, float b, float a = 0) noexcept
	{
		const FLOAT color[4] = { r, g, b, a };

		pCTX->ClearRenderTargetView(pTarget, color);
	}

	void EndFrame()
	{
		pSwapChain->Present(2, 0);
	}

private:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pCTX;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pTarget;

	ID3D11RenderTargetView* _Create_render_target(IDXGISwapChain* _Swap_chain) noexcept
	{
		ID3D11Resource* pBackBuffer = nullptr;
		HRESULT ok = _Swap_chain->GetBuffer(DXGI_SWAP_EFFECT_DISCARD,
			__uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
		
		if (ok != S_OK || !pBackBuffer)
			return nullptr;

		ID3D11RenderTargetView* pRenderTarget = nullptr;
		ok = pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTarget);

		if (ok != S_OK || !pRenderTarget)
			return nullptr;

		_SafeRelease(pBackBuffer);
		return pRenderTarget;
	}

	template <class _Interf>
	void _SafeRelease(_Interf& _Obj) const noexcept
	{
		if (_Obj)
		{
			_Obj->Release();

			_Obj = nullptr;
		}
	}
};