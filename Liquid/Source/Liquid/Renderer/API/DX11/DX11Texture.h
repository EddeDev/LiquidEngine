#pragma once

#include "Liquid/Renderer/API/Texture.h"

#include "DX11Common.h"

namespace Liquid {

	class DX11Texture : public Texture
	{
	public:
		DX11Texture(const String& filepath);
		virtual ~DX11Texture();

		ID3D11Texture2D* GetTexture() const { return m_Texture; }
		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_ShaderResourceView; }
	private:
		ID3D11Texture2D* m_Texture = nullptr;
		ID3D11ShaderResourceView* m_ShaderResourceView = nullptr;
		ID3D11SamplerState* m_SamplerState = nullptr;
	};

}