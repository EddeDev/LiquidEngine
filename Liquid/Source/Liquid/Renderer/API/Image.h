#pragma once

#include "Liquid/Renderer/RenderResource.h"
#include "Liquid/Renderer/PixelFormat.h"

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
		virtual void Bind(uint32 slot = 0) const = 0;
		virtual void Unbind(uint32 slot = 0) const = 0;
	};

	class Image2D : public Image
	{
	public:
		static Ref<Image2D> Create(const ImageCreateInfo& createInfo);
	};

}