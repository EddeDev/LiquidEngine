#pragma once

#include "Liquid/Renderer/API/GraphicsContext.h"

#include "DX11Common.h"
#include "DX11Device.h"

namespace Liquid {

	class DX11Context : public GraphicsContext
	{
	public:
		DX11Context(const GraphicsContextCreateInfo& createInfo);
		virtual ~DX11Context() {}
	private:
		GraphicsContextCreateInfo m_CreateInfo;
	};

}