#pragma once

#include "Liquid/Core/RefCounting.h"

namespace Liquid {

	class RenderResource : public RefCounted
	{
	public:
		virtual ~RenderResource() {}
	};

}