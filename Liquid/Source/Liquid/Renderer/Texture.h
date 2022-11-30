#pragma once

#include "API/Image.h"

namespace Liquid {

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
		Texture2D(const std::filesystem::path& path);

		virtual void Bind(uint32 slot = 0) const override;
		virtual void Unbind(uint32 slot = 0) const override;

		Ref<Image2D> GetImage() const { return m_Image; }
	private:
		Ref<Image2D> m_Image;
	};

}