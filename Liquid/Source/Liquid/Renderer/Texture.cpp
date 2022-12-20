#include "LiquidPCH.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Liquid {

	Texture2D::Texture2D(const String& path)
	{
		if (!std::filesystem::exists(path))
		{
			LQ_ERROR_CATEGORY("Texture2D", "Could not find texture '{0}'", path);
			return;
		}

		stbi_set_flip_vertically_on_load(true);

		int32 width, height;
		uint8* data = stbi_load(path.c_str(), &width, &height, nullptr, STBI_rgb_alpha);
		if (!data)
		{
			String errorMessage = stbi_failure_reason();
			LQ_ERROR_CATEGORY("Texture2D", "STBI: {0}", errorMessage);
			return;
		}

		ImageCreateInfo imageCreateInfo;
		imageCreateInfo.Data = data;
		imageCreateInfo.Format = PixelFormat::RGBA;
		imageCreateInfo.Width = width;
		imageCreateInfo.Height = height;

		m_Image = Image2D::Create(imageCreateInfo);

		stbi_image_free(data);
	}

	Texture2D::Texture2D(const void* data, uint32 size)
	{
		stbi_set_flip_vertically_on_load(true);

		int32 width, height;
		uint8* imageData = stbi_load_from_memory((const stbi_uc*)data, size, &width, &height, nullptr, STBI_rgb_alpha);
		if (!data)
		{
			String errorMessage = stbi_failure_reason();
			LQ_ERROR_CATEGORY("Texture2D", "STBI: {0}", errorMessage);
			return;
		}

		ImageCreateInfo imageCreateInfo;
		imageCreateInfo.Data = imageData;
		imageCreateInfo.Format = PixelFormat::RGBA;
		imageCreateInfo.Width = width;
		imageCreateInfo.Height = height;

		m_Image = Image2D::Create(imageCreateInfo);

		stbi_image_free(imageData);
	}

	Texture2D::Texture2D(uint32 width, uint32 height, PixelFormat format, const void* data)
	{
		ImageCreateInfo imageCreateInfo;
		imageCreateInfo.Data = data;
		imageCreateInfo.Format = format;
		imageCreateInfo.Width = width;
		imageCreateInfo.Height = height;

		m_Image = Image2D::Create(imageCreateInfo);
	}

	void Texture2D::Bind(uint32 slot) const
	{
		m_Image->Bind(slot);
	}

	void Texture2D::Unbind(uint32 slot) const
	{
		m_Image->Unbind(slot);
	}

}