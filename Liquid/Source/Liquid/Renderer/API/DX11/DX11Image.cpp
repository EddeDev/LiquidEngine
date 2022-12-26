#include "LiquidPCH.h"
#include "DX11Image.h"

#include "Liquid/Renderer/RenderThread.h"

#include "DX11Device.h"
#include "DX11PixelFormat.h"

namespace Liquid {

	static ID3D11ShaderResourceView* s_NullShaderResourceView = nullptr;

	DX11Image2D::DX11Image2D(const ImageCreateInfo& createInfo)
		: m_Width(createInfo.Width), m_Height(createInfo.Height), m_Format(createInfo.Format)
	{
		uint32 dataSize = m_Width * m_Height * PixelFormatUtils::ComputeBytesPerPixel(m_Format);
		LQ_CHECK(dataSize > 0);

		m_Storage.Allocate(dataSize);
		if (createInfo.Data)
			m_Storage.Insert(createInfo.Data, dataSize);
		else
			m_Storage.FillWithZeroes();

		Invalidate();
	}

	DX11Image2D::~DX11Image2D()
	{
		Release();
	}

	void DX11Image2D::Invalidate()
	{
		Ref<DX11Image2D> instance = this;
		RT_SUBMIT(Invalidate)([instance]() mutable
		{
			instance->RT_Invalidate();
		});
	}

	void DX11Image2D::RT_Invalidate()
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = m_Width;
		textureDesc.Height = m_Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DX11PixelFormat(m_Format);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subresource = {};
		subresource.pSysMem = m_Storage.GetData();
		subresource.SysMemPitch = textureDesc.Width * PixelFormatUtils::ComputeBytesPerPixel(m_Format);
		subresource.SysMemSlicePitch = 0;
		DX_CHECK(device->CreateTexture2D(&textureDesc, &subresource, &m_Texture));

		if (textureDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = DX11SRVFormat(m_Format);
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			DX_CHECK(device->CreateShaderResourceView(m_Texture, &srvDesc, &m_ShaderResourceView));
		}
	}

	void DX11Image2D::Release()
	{
		RT_SUBMIT_RELEASE(Release)([texture = m_Texture, shaderResourceView = m_ShaderResourceView]()
		{
			if (texture)
				texture->Release();
			if (shaderResourceView)
				shaderResourceView->Release();
		});

		m_Storage.Release();
	}

	void DX11Image2D::RT_Release()
	{
		if (m_Texture)
			m_Texture->Release();
		if (m_ShaderResourceView)
			m_ShaderResourceView->Release();

		m_Storage.Release();
	}

	void DX11Image2D::Bind(uint32 slot, ShaderStage stage) const
	{
		Ref<const DX11Image2D> instance = this;
		RT_SUBMIT(Bind)([instance, slot, stage]()
		{
			instance->RT_Bind(slot, stage);
		});
	}

	void DX11Image2D::RT_Bind(uint32 slot, ShaderStage stage) const
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		switch (stage)
		{
		case ShaderStage::Vertex:
		{
			deviceContext->VSSetShaderResources(slot, 1, &m_ShaderResourceView);
			break;
		}
		case ShaderStage::Pixel:
		{
			deviceContext->PSSetShaderResources(slot, 1, &m_ShaderResourceView);
			break;
		}
		case ShaderStage::Compute:
		{
			deviceContext->CSSetShaderResources(slot, 1, &m_ShaderResourceView);
			break;
		}
		}
	}

	void DX11Image2D::Unbind(uint32 slot, ShaderStage stage) const
	{
		Ref<const DX11Image2D> instance = this;
		RT_SUBMIT(Unbind)([instance, slot, stage]()
		{
			instance->RT_Unbind(slot, stage);
		});
	}

	void DX11Image2D::RT_Unbind(uint32 slot, ShaderStage stage) const
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		switch (stage)
		{
		case ShaderStage::Vertex:
		{
			deviceContext->VSSetShaderResources(slot, 1, &s_NullShaderResourceView);
			break;
		}
		case ShaderStage::Pixel:
		{
			deviceContext->PSSetShaderResources(slot, 1, &s_NullShaderResourceView);
			break;
		}
		case ShaderStage::Compute:
		{
			deviceContext->CSSetShaderResources(slot, 1, &s_NullShaderResourceView);
			break;
		}
		}
	}

}