#pragma once

#include "Liquid/Renderer/API/GraphicsDevice.h"

#include "DX11Common.h"

namespace Liquid {

	class DX11Device : public GraphicsDevice
	{
	public:
		DX11Device(const GraphicsDeviceCreateInfo& createInfo, DXRef<IDXGIAdapter> adapter);
		virtual ~DX11Device();

		virtual const GraphicsDeviceInfo& GetInfo() const override { return m_Info; }

		DXRef<ID3D11Device> GetDevice() const { return m_Device; }
		DXRef<ID3D11DeviceContext> GetDeviceContext() const { return m_DeviceContext; }

		inline static DX11Device& Get()
		{
			LQ_ASSERT(s_Instance, "Instance is nullptr");
			return *s_Instance;
		}

		static Ref<DX11Device> Select(const GraphicsDeviceCreateInfo& createInfo);
	private:
		inline static DX11Device* s_Instance = nullptr;

		GraphicsDeviceInfo m_Info;
		
		D3D_FEATURE_LEVEL m_FeatureLevel;

		DXRef<ID3D11Device> m_Device;
		DXRef<ID3D11DeviceContext> m_DeviceContext;
	};

}