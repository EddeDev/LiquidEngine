#include "LiquidPCH.h"
#include "DX11Texture.h"

#include "DX11Device.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Liquid {

	DX11Texture::DX11Texture(const String& filepath)
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		stbi_set_flip_vertically_on_load(true);

		int32 width, height;
		uint8* pixels = stbi_load(filepath.c_str(), &width, &height, nullptr, STBI_rgb_alpha);
		LQ_ASSERT(pixels, "STBI: {0}", stbi_failure_reason());

		// Texture
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;

			ID3D11Texture2D* pTexture = nullptr;
			D3D11_SUBRESOURCE_DATA subResource;
			subResource.pSysMem = pixels;
			subResource.SysMemPitch = desc.Width * 4;
			subResource.SysMemSlicePitch = 0;
			DX_CHECK(device->CreateTexture2D(&desc, &subResource, &pTexture));

			// Default view
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = desc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
			device->CreateShaderResourceView(pTexture, &srvDesc, &m_ShaderResourceView);
			pTexture->Release();
		}

		// Sampler
		{
			D3D11_SAMPLER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.MipLODBias = 0.f;
			desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			desc.MinLOD = 0.f;
			desc.MaxLOD = 0.f;
			device->CreateSamplerState(&desc, &m_SamplerState);
		}
	}

	DX11Texture::~DX11Texture()
	{

	}

}