#pragma once

#include "Liquid/Renderer/RenderResource.h"
#include "Liquid/Renderer/PixelFormat.h"

#include "Liquid/Renderer/API/Shader.h"

namespace Liquid {

	struct ImageCreateInfo
	{
		const void* Data = nullptr;
		PixelFormat Format = PixelFormat::RGBA;

		uint32 Width = 1;
		uint32 Height = 1;
	};

	class Image : public RenderResource
	{
	public:
		virtual void Invalidate() = 0;
		virtual void RT_Invalidate() = 0;

		virtual void Release() = 0;
		virtual void RT_Release() = 0;

		virtual void Bind(uint32 slot = 0, ShaderStage stage = ShaderStage::Fragment) const = 0;
		virtual void Unbind(uint32 slot = 0, ShaderStage stage = ShaderStage::Fragment) const = 0;

		virtual void RT_Bind(uint32 slot = 0, ShaderStage stage = ShaderStage::Fragment) const = 0;
		virtual void RT_Unbind(uint32 slot = 0, ShaderStage stage = ShaderStage::Fragment) const = 0;
	};

	class Image2D : public Image
	{
	public:
		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		RENDER_RESOURCE_TYPE(Image2D);

		static Ref<Image2D> Create(const ImageCreateInfo& createInfo);
	};

}