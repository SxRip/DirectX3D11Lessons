#pragma once
#include "BaseException.hpp"
#include "dxerr.h"
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

class Graphics
{
public:
	class GFXException : public BaseException
	{
	public:
		GFXException(const char* file, size_t line, HRESULT hr) : BaseException(file, line, hr)
		{}

		inline virtual const char* what()					const noexcept override;
		inline virtual const char* get_type()				const noexcept override;

		inline HRESULT				get_error_code()		const noexcept { return _hr; }
		inline std::string			get_error_string()		const noexcept { return DXGetErrorString(_hr); }
		inline std::string			get_error_description() const noexcept;
	};

	class RemovedDeviceEx : public BaseException
	{
	public:
		RemovedDeviceEx(const char* file, size_t line, HRESULT hr) :
			BaseException(file, line, hr)
		{}

		inline virtual const char* what() const noexcept override;
		inline virtual const char* get_type() const noexcept override { return "[Removed device exception]"; }
	};

	Graphics(HWND hwnd);

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

	void EndFrame();

private:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pCTX;
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pTarget;

	ID3D11RenderTargetView* _Create_render_target(IDXGISwapChain* _Swap_chain);

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