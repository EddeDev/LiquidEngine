#pragma once

#include "Logger.h"

#define LQ_TRACE_ARGS(...)    LQ_LOG_ARGS(::Liquid::LogVerbosity::Trace,    __VA_ARGS__)
#define LQ_INFO_ARGS(...)     LQ_LOG_ARGS(::Liquid::LogVerbosity::Info,     __VA_ARGS__)
#define LQ_WARNING_ARGS(...)  LQ_LOG_ARGS(::Liquid::LogVerbosity::Warning,  __VA_ARGS__)
#define LQ_ERROR_ARGS(...)    LQ_LOG_ARGS(::Liquid::LogVerbosity::Error,    __VA_ARGS__)
#define LQ_CRITICAL_ARGS(...) LQ_LOG_ARGS(::Liquid::LogVerbosity::Critical, __VA_ARGS__)

#define LQ_TRACE_CATEGORY(category, ...)    LQ_LOG_CATEGORY(::Liquid::LogVerbosity::Trace,    category, __VA_ARGS__)
#define LQ_INFO_CATEGORY(category, ...)     LQ_LOG_CATEGORY(::Liquid::LogVerbosity::Info,     category, __VA_ARGS__)
#define LQ_WARNING_CATEGORY(category, ...)  LQ_LOG_CATEGORY(::Liquid::LogVerbosity::Warning,  category, __VA_ARGS__)
#define LQ_ERROR_CATEGORY(category, ...)    LQ_LOG_CATEGORY(::Liquid::LogVerbosity::Error,    category, __VA_ARGS__)
#define LQ_CRITICAL_CATEGORY(category, ...) LQ_LOG_CATEGORY(::Liquid::LogVerbosity::Critical, category, __VA_ARGS__)

#define LQ_TRACE_PREFIX(prefix, ...)    LQ_LOG_PREFIX(::Liquid::LogVerbosity::Trace,    prefix, __VA_ARGS__)
#define LQ_INFO_PREFIX(prefix, ...)     LQ_LOG_PREFIX(::Liquid::LogVerbosity::Info,     prefix, __VA_ARGS__)
#define LQ_WARNING_PREFIX(prefix, ...)  LQ_LOG_PREFIX(::Liquid::LogVerbosity::Warning,  prefix, __VA_ARGS__)
#define LQ_ERROR_PREFIX(prefix, ...)    LQ_LOG_PREFIX(::Liquid::LogVerbosity::Error,    prefix, __VA_ARGS__)
#define LQ_CRITICAL_PREFIX(prefix, ...) LQ_LOG_PREFIX(::Liquid::LogVerbosity::Critical, prefix, __VA_ARGS__)