#pragma once

#include "Liquid/Renderer/API/GraphicsContext.h"

#include "DX11Common.h"

namespace Liquid {

	class DX11Context : public GraphicsContext
	{
	public:
		DX11Context(const GraphicsContextCreateInfo& createInfo);
		virtual ~DX11Context() {}

		DXRef<ID3D11Device> GetDevice() const { return m_Device; }
		DXRef<ID3D11DeviceContext> GetDeviceContext() const { return m_DeviceContext; }
	private:
		GraphicsContextCreateInfo m_CreateInfo;

		D3D_FEATURE_LEVEL m_FeatureLevel;

		DXRef<ID3D11Device> m_Device;
		DXRef<ID3D11DeviceContext> m_DeviceContext;
		DXRef<ID3D11Debug> m_DebugLayer;
	};

}