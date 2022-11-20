#pragma once

#include "Liquid/Core/String.h"

#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <dxgi.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>

#include <wrl/client.h>
#include <comdef.h>

namespace Liquid {

	template<typename T>
	using DXRef = Microsoft::WRL::ComPtr<T>;
	
#define DX_RIID(i) __uuidof(i)

#ifdef LQ_BUILD_DEBUG
	#define DX_CHECKS
#endif

#ifdef DX_CHECKS
	#define DX_CHECK(hr) \
	{ \
		if (!SUCCEEDED(hr)) \
		{ \
			_com_error error(hr); \
			LPCTSTR errorMessage = error.ErrorMessage(); \
			/* LQ_ASSERT(false, StringUtils::FromWideString(errorMessage).c_str()); */ \
		} \
	}
#else
	#define DX_CHECK(hr) hr
#endif

}