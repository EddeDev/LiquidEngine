#pragma once

#include "API/Image.h"

namespace Liquid {

	// Texture asset

	class Texture : public RefCounted
	{
	public:
		virtual ~Texture() {}

		virtual void Bind(uint32 slot = 0) const = 0;
		virtual void Unbind(uint32 slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const String& path);
		Texture2D(uint32 width, uint32 height, PixelFormat format = PixelFormat::RGBA, const void* data = nullptr);

		virtual void Bind(uint32 slot = 0) const override;
		virtual void Unbind(uint32 slot = 0) const override;

		uint32 GetWidth() const { return m_Image->GetWidth(); }
		uint32 GetHeight() const { return m_Image->GetHeight(); }

		Ref<Image2D> GetImage() const { return m_Image; }
	private:
		Ref<Image2D> m_Image;
	};

}