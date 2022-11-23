#include "LiquidPCH.h"

#ifdef LQ_PLATFORM_WINDOWS

#include "Liquid/Launch/CommandLineArgs.h"

// TODO: move to platform utils
#ifdef UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

namespace Liquid {

	extern int32 Main(const CommandLineArgs& args);

}

#ifdef LQ_BUILD_SHIPPING
	#ifdef _UNICODE
		int WINAPI wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ PWSTR cmdLine, _In_ int showCmd)
		{
			return Liquid::Main({ __argc, __argv });
		}
	#else
		int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ PSTR cmdLine, _In_ int showCmd)
		{
			return Liquid::Main({ __argc, __argv });
		}
	#endif
#else
	int main(int argc, char* argv[])
	{
		return Liquid::Main({ argc, argv });
	}
#endif

#endif