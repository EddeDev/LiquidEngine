#pragma once

#include "Liquid/Renderer/API/Swapchain.h"

#include "DX11Common.h"

namespace Liquid {

	class DX11Swapchain : public Swapchain
	{
	public:
		DX11Swapchain(const SwapchainCreateInfo& createInfo);
		virtual ~DX11Swapchain() {}

		virtual void Resize(uint32 width, uint32 height) override;
		virtual void Present() const override;
	private:
		SwapchainCreateInfo m_CreateInfo;

		DXRef<IDXGISwapChain> m_SwapChain;

		DXRef<ID3D11RenderTargetView> m_BackBuffer;
		DXRef<ID3D11Texture2D> m_DepthStencilBuffer;
		DXRef<ID3D11DepthStencilView> m_DepthStencilView;
	};

}