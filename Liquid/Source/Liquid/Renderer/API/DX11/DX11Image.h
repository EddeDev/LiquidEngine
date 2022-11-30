#pragma once

#include "Liquid/Renderer/API/Image.h"

#include "DX11Common.h"

namespace Liquid {

	class DX11Image2D : public Image2D
	{
	public:
		DX11Image2D(const ImageCreateInfo& createInfo);
		virtual ~DX11Image2D();

		virtual void Bind(uint32 slot = 0) const override;
		virtual void Unbind(uint32 slot = 0) const override;

		ID3D11Texture2D* GetTexture() const { return m_Texture; }
		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_ShaderResourceView; }
	private:
		ID3D11Texture2D* m_Texture = nullptr;
		ID3D11ShaderResourceView* m_ShaderResourceView = nullptr;
	};

}