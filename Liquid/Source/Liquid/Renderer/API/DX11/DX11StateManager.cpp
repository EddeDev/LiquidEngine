#include "LiquidPCH.h"
#include "DX11StateManager.h"

#include "DX11Device.h"

namespace Liquid {

	struct DX11StateManagerData
	{
		// Null resource views
		static constexpr ID3D11ShaderResourceView* NullShaderResourceView = nullptr;
		static constexpr ID3D11UnorderedAccessView* NullUnorderedAccessView = nullptr;
		static constexpr ID3D11RenderTargetView* NullRenderTargetView = nullptr;
		static constexpr ID3D11DepthStencilView* NullDepthStencilView = nullptr;

		// Shader
		ID3D11DeviceChild* CurrentlyBoundVS = nullptr;
		ID3D11DeviceChild* CurrentlyBoundPS = nullptr;
		ID3D11DeviceChild* CurrentlyBoundCS = nullptr;

		// Resource views
		ID3D11ShaderResourceView* CurrentlyBoundSRV = nullptr;
		ID3D11UnorderedAccessView* CurrentlyBoundUAV = nullptr;
		ID3D11DepthStencilView* CurrentlyBoundDSV = nullptr;
		std::array<ID3D11RenderTargetView*, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT> CurrentlyBoundRTVs;
		int32 CurrentlyBoundResourceSlot = -1;

		// Buffers
		ID3D11Buffer* CurrentlyBoundBuffer = nullptr;
	};

	static DX11StateManagerData s_Data;

	void DX11StateManager::BindShader(ShaderStage stage, ID3D11DeviceChild* shader)
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		switch (stage)
		{
		case ShaderStage::Vertex:
		{
			LQ_CHECK(!s_Data.CurrentlyBoundVS);
			deviceContext->VSSetShader(static_cast<ID3D11VertexShader*>(shader), nullptr, 0);
			s_Data.CurrentlyBoundVS = shader;
			break;
		}
		case ShaderStage::Pixel:
		{
			LQ_CHECK(!s_Data.CurrentlyBoundPS);
			deviceContext->PSSetShader(static_cast<ID3D11PixelShader*>(shader), nullptr, 0);
			s_Data.CurrentlyBoundPS = shader;
			break;
		}
		case ShaderStage::Compute:
		{
			LQ_CHECK(!s_Data.CurrentlyBoundCS);
			deviceContext->CSSetShader(static_cast<ID3D11ComputeShader*>(shader), nullptr, 0);
			s_Data.CurrentlyBoundCS = shader;
			break;
		}
		}
	}

	void DX11StateManager::UnbindShader(ShaderStage stage)
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		switch (stage)
		{
		case ShaderStage::Vertex:
		{
			LQ_CHECK(s_Data.CurrentlyBoundVS);
			deviceContext->VSSetShader(nullptr, nullptr, 0);
			s_Data.CurrentlyBoundVS = nullptr;
			break;
		}
		case ShaderStage::Pixel:
		{
			LQ_CHECK(s_Data.CurrentlyBoundPS);
			deviceContext->PSSetShader(nullptr, nullptr, 0);
			s_Data.CurrentlyBoundPS = nullptr;
			break;
		}
		case ShaderStage::Compute:
		{
			LQ_CHECK(s_Data.CurrentlyBoundCS);
			deviceContext->CSSetShader(nullptr, nullptr, 0);
			s_Data.CurrentlyBoundCS = nullptr;
			break;
		}
		}
	}

	void DX11StateManager::BindShaderResourceView(ShaderStage stage, ID3D11ShaderResourceView* shaderResourceView, uint32 slot)
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();
		
		LQ_CHECK(!s_Data.CurrentlyBoundSRV);
		LQ_CHECK(s_Data.CurrentlyBoundResourceSlot == -1);

		switch (stage)
		{
		case ShaderStage::Vertex:
		{
			deviceContext->VSSetShaderResources(slot, 1, &shaderResourceView);
			break;
		}
		case ShaderStage::Pixel:
		{
			deviceContext->PSSetShaderResources(slot, 1, &shaderResourceView);
			break;
		}
		case ShaderStage::Compute:
		{
			deviceContext->CSSetShaderResources(slot, 1, &shaderResourceView);
			break;
		}
		}

		s_Data.CurrentlyBoundSRV = shaderResourceView;
		s_Data.CurrentlyBoundResourceSlot = slot;
	}

	void DX11StateManager::UnbindShaderResourceView(ShaderStage stage, uint32 slot)
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		LQ_CHECK(s_Data.CurrentlyBoundSRV);
		LQ_CHECK(s_Data.CurrentlyBoundResourceSlot != -1);

		switch (stage)
		{
		case ShaderStage::Vertex:
		{
			deviceContext->VSSetShaderResources(slot, 1, &s_Data.NullShaderResourceView);
			break;
		}
		case ShaderStage::Pixel:
		{
			deviceContext->PSSetShaderResources(slot, 1, &s_Data.NullShaderResourceView);
			break;
		}
		case ShaderStage::Compute:
		{
			deviceContext->CSSetShaderResources(slot, 1, &s_Data.NullShaderResourceView);
			break;
		}
		}

		s_Data.CurrentlyBoundSRV = nullptr;
		s_Data.CurrentlyBoundResourceSlot = -1;
	}

	void DX11StateManager::BindUnorderedAccessView(ID3D11UnorderedAccessView* unorderedAccessView, uint32 slot)
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		LQ_CHECK(!s_Data.CurrentlyBoundUAV);
		LQ_CHECK(s_Data.CurrentlyBoundResourceSlot == -1);

		deviceContext->CSSetUnorderedAccessViews(slot, 1, &unorderedAccessView, nullptr);

		s_Data.CurrentlyBoundUAV = unorderedAccessView;
		s_Data.CurrentlyBoundResourceSlot = slot;
	}

	void DX11StateManager::UnbindUnorderedAccessView(uint32 slot)
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		LQ_CHECK(s_Data.CurrentlyBoundUAV);
		LQ_CHECK(s_Data.CurrentlyBoundResourceSlot != -1);

		deviceContext->CSSetUnorderedAccessViews(slot, 1, &s_Data.NullUnorderedAccessView, nullptr);

		s_Data.CurrentlyBoundUAV = nullptr;
		s_Data.CurrentlyBoundResourceSlot = -1;
	}

	void DX11StateManager::BindRenderTargetViews(const std::vector<ID3D11RenderTargetView*>& renderTargetViews)
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		LQ_CHECK(s_Data.CurrentlyBoundDSV);
		
		for (size_t i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			LQ_CHECK(!s_Data.CurrentlyBoundRTVs[i]);

		deviceContext->OMSetRenderTargets(static_cast<uint32>(renderTargetViews.size()), renderTargetViews.data(), s_Data.CurrentlyBoundDSV);

		// Copy resource views
		for (size_t i = 0; i < renderTargetViews.size(); i++)
			s_Data.CurrentlyBoundRTVs[i] = renderTargetViews[i];
	}

	void DX11StateManager::UnbindRenderTargetViews()
	{
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		deviceContext->OMSetRenderTargets(1, &s_Data.NullRenderTargetView, nullptr);

		// Clear array
		for (size_t i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			s_Data.CurrentlyBoundRTVs[i] = nullptr;
	}

}