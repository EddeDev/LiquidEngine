#include "LiquidPCH.h"
#include "DX11Image.h"

#include "DX11Device.h"
#include "DX11PixelFormat.h"

namespace Liquid {

	DX11Image2D::DX11Image2D(const ImageCreateInfo& createInfo)
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = createInfo.Width;
		textureDesc.Height = createInfo.Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DX11PixelFormat(createInfo.Format);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subresource = {};
		subresource.pSysMem = createInfo.Data;
		subresource.SysMemPitch = textureDesc.Width * PixelFormatUtils::GetBPP(createInfo.Format);
		subresource.SysMemSlicePitch = 0;
		DX_CHECK(device->CreateTexture2D(&textureDesc, &subresource, &m_Texture));

		if (textureDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = DX11SRVFormat(createInfo.Format);
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			DX_CHECK(device->CreateShaderResourceView(m_Texture, &srvDesc, &m_ShaderResourceView));
		}
	}

	DX11Image2D::~DX11Image2D()
	{
		m_Texture->Release();
		m_ShaderResourceView->Release();
	}

	void DX11Image2D::Bind(uint32 slot) const
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		deviceContext->PSSetShaderResources(slot, 1, &m_ShaderResourceView);
	}

	void DX11Image2D::Unbind(uint32 slot) const
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		ID3D11ShaderResourceView* nullSRV = nullptr;
		deviceContext->PSSetShaderResources(slot, 1, &nullSRV);
	}

}