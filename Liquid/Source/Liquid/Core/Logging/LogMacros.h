#pragma once

#include "Logger.h"

#if !defined(LQ_BUILD_SHIPPING)
#define LQ_TRACE_ENABLED
#endif

#ifdef LQ_TRACE_ENABLED
#define LQ_TRACE_ARGS(...) LQ_LOG_ARGS(Trace, __VA_ARGS__)
#define LQ_TRACE_CATEGORY(category, ...) LQ_LOG_CATEGORY(Trace, category, __VA_ARGS__)
#define LQ_TRACE_PREFIX(prefix, ...) LQ_LOG_PREFIX(Trace, prefix, __VA_ARGS__)
#else
#define LQ_TRACE_ARGS(...)
#define LQ_TRACE_CATEGORY(category, ...)
#define LQ_TRACE_PREFIX(prefix, ...)
#endif

#define LQ_INFO_ARGS(...)     LQ_LOG_ARGS(Info,     __VA_ARGS__)
#define LQ_WARNING_ARGS(...)  LQ_LOG_ARGS(Warning,  __VA_ARGS__)
#define LQ_ERROR_ARGS(...)    LQ_LOG_ARGS(Error,    __VA_ARGS__)
#define LQ_CRITICAL_ARGS(...) LQ_LOG_ARGS(Critical, __VA_ARGS__)

#define LQ_INFO_CATEGORY(category, ...)     LQ_LOG_CATEGORY(Info,     category, __VA_ARGS__)
#define LQ_WARNING_CATEGORY(category, ...)  LQ_LOG_CATEGORY(Warning,  category, __VA_ARGS__)
#define LQ_ERROR_CATEGORY(category, ...)    LQ_LOG_CATEGORY(Error,    category, __VA_ARGS__)
#define LQ_CRITICAL_CATEGORY(category, ...) LQ_LOG_CATEGORY(Critical, category, __VA_ARGS__)

#define LQ_INFO_PREFIX(prefix, ...)     LQ_LOG_PREFIX(Info,     prefix, __VA_ARGS__)
#define LQ_WARNING_PREFIX(prefix, ...)  LQ_LOG_PREFIX(Warning,  prefix, __VA_ARGS__)
#define LQ_ERROR_PREFIX(prefix, ...)    LQ_LOG_PREFIX(Error,    prefix, __VA_ARGS__)
#define LQ_CRITICAL_PREFIX(prefix, ...) LQ_LOG_PREFIX(Critical, prefix, __VA_ARGS__)