#pragma once
#include "BaseException.hpp"
#include "dxerr.h"
#include "DXGIInfoManager.hpp"
#include <d3d11.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")

template <class _DirectXInterf>
void SafeRelease(_DirectXInterf& _Obj)
{
	if (_Obj)
	{
		_Obj->Release();

		_Obj = nullptr;
	}
}

class Graphics
{
public:
	class GFXException : public BaseException
	{
	public:
		GFXException(const char* file, size_t line, HRESULT hr) : BaseException(file, line, hr)
		{}

		GFXException(const char* file, size_t line, HRESULT hr, std::vector<std::string>& messages) : BaseException(file, line, hr)
		{
			for (const auto& str : messages)
			{
				_info = str;
				_info.push_back('\n');
			}

			if (!_info.empty())
				_info.pop_back();
		}

		GFXException(const char* file, size_t line, HRESULT hr, std::vector<std::string>&& messages)
			: GFXException(file, line, hr, messages)
		{}

		inline virtual const char*	what()					const noexcept override;
		inline virtual const char*	get_type()				const noexcept override;

		inline HRESULT				get_error_code()		const noexcept { return _hr; }
		inline std::string			get_error_string()		const noexcept { return DXGetErrorString(_hr); }
		inline std::string			get_error_description() const noexcept;
	private:
		std::string _info;
	};

	class RemovedDeviceEx : public GFXException
	{
	public:
		RemovedDeviceEx(const char* file, size_t line, HRESULT hr) :
			GFXException(file, line, hr)
		{}

		RemovedDeviceEx(const char* file, size_t line, HRESULT hr, std::vector<std::string>&& messages) :
			GFXException(file, line, hr, messages)
		{}

		inline virtual const char* what()					const noexcept override;
		inline virtual const char* get_type()				const noexcept override { return "[Removed device exception]"; }
	private:
		std::string _reason;
	};

	Graphics(HWND hwnd);
	~Graphics() = default;

	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void clearWindow(float r, float g, float b, float a = 0) noexcept
	{
		const FLOAT color[4] = { r, g, b, a };

		pCTX->ClearRenderTargetView(pTarget.Get(), color);
	}

	void EndFrame();

private:
#ifdef _DEBUG
	DXGIInfoManager _DXGIInfo;
#endif // DEBUG


	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> pCTX;
	Microsoft::WRL::ComPtr <IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> pTarget;
};