#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <queue>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <unordered_set>

#include "Liquid/Core/Logging/LogMacros.h"
#include "Liquid/Core/AssertionMacros.h"
#include "Liquid/Core/PrimitiveTypes.h"
#include "Liquid/Core/RefCounting.h"

#ifdef LQ_PLATFORM_WINDOWS
#include <Windows.h>
#endif