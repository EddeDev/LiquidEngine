#pragma once

#include "Liquid/Renderer/RenderResource.h"

namespace Liquid {

	enum class ShaderStage : uint8
	{
		Vertex,
		Fragment,
		Compute
	};

	struct ShaderCreateInfo
	{

	};

	class Shader : public RenderResource
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void RT_Bind() = 0;
		virtual void RT_Unbind() = 0;
	};

}