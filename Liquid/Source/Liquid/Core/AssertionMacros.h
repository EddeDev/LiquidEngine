#pragma once

#include "Logging/Logger.h"

#if defined(LQ_PLATFORM_WINDOWS)
	#define LQ_PLATFORM_BREAK() __debugbreak()
#elif defined(LQ_PLATFORM_LINUX)
	#inclide <signal.h>
	#define LQ_PLATFORM_BREAK() raise(SIGTRAP)
#else
	#define LQ_PLATFORM_BREAK()
#endif

#ifdef LQ_BUILD_DEBUG
	#define LQ_ENABLE_ASSERTS
	#define LQ_ENABLE_CHECKS
#endif

#ifdef LQ_ENABLE_CHECKS
	#define LQ_CHECK(expression) if (!(expression)) LQ_PLATFORM_BREAK()
#else
	#define LQ_CHECK(expression)
#endif

#ifdef LQ_ENABLE_ASSERTS
	#define LQ_ASSERT(expression, ...) \
	{ \
		if (!(expression)) \
		{ \
			LQ_LOG_PREFIX(Error, "Assertion failed", __VA_ARGS__); \
			LQ_PLATFORM_BREAK(); \
		} \
	}
#else
	#define LQ_ASSERT(expression, ...)
#endif

#ifndef LQ_BUILD_SHIPPING
	#define LQ_VERIFY(expression, ...) \
	{ \
		if (!(expression)) \
		{ \
			LQ_LOG_PREFIX(Error, "Verify failed", __VA_ARGS__); \
			LQ_PLATFORM_BREAK(); \
		} \
	}
#else
	#define LQ_VERIFY(expression, ...)
#endif
