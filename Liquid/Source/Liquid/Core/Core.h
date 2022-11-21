#pragma once

#include "Logging/LogMacros.h"
#include "AssertionMacros.h"
#include "PrimitiveTypes.h"
#include "RefCounting.h"
#include "String.h"

#define LQ_BIND_CALLBACK(func, ...) \
[__VA_ARGS__](auto&&... args) -> decltype(auto) \
{ \
	return func(std::forward<decltype(args)>(args)...); \
}

namespace Liquid {

#define BIT(x) 1 << x

}