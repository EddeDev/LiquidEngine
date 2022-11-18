#pragma once

#include "Liquid/Core/PrimitiveTypes.h"

namespace Liquid {

	struct CommandLineArgs
	{
		int32 Count = 0;
		char** Args = nullptr;

		const char* operator[](uint32 index) const
		{
			return Args[index];
		}
	};

}