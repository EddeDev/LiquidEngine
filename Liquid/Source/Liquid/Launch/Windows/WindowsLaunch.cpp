#include "LiquidPCH.h"

#ifdef LQ_PLATFORM_WINDOWS

#include "Liquid/Launch/CommandLineArgs.h"

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