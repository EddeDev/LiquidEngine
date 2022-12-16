#pragma once

#include "Liquid/Renderer/RenderResource.h"

namespace Liquid {

	enum class ShaderStage : uint8
	{
		None = 0,

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
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void RT_Bind() const = 0;
		virtual void RT_Unbind() const = 0;

		RENDER_RESOURCE_TYPE(Shader);
	};

}