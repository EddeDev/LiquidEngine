#pragma once

#include "Liquid/Renderer/API/Image.h"

#include "DX11Common.h"

namespace Liquid {

	class DX11Image2D : public Image2D
	{
	public:
		DX11Image2D(const ImageCreateInfo& createInfo);
		virtual ~DX11Image2D();

		virtual void Invalidate() override;
		virtual void RT_Invalidate() override;

		virtual void Release() override;
		virtual void RT_Release() override;

		virtual void Bind(uint32 slot = 0, ShaderStage stage = ShaderStage::Pixel) const override;
		virtual void Unbind(uint32 slot = 0, ShaderStage stage = ShaderStage::Pixel) const override;

		virtual void RT_Bind(uint32 slot = 0, ShaderStage stage = ShaderStage::Pixel) const override;
		virtual void RT_Unbind(uint32 slot = 0, ShaderStage stage = ShaderStage::Pixel) const override;

		virtual uint32 GetWidth() const override { return m_Width; }
		virtual uint32 GetHeight() const override { return m_Height; }

		ID3D11Texture2D* GetTexture() const { return m_Texture; }
		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_ShaderResourceView; }
	private:
		uint32 m_Width = 0;
		uint32 m_Height = 0;
		PixelFormat m_Format = PixelFormat::None;

		DynamicBuffer m_Storage;

		ID3D11Texture2D* m_Texture = nullptr;
		ID3D11ShaderResourceView* m_ShaderResourceView = nullptr;
	};

}