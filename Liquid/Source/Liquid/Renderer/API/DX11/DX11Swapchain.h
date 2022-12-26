#pragma once

#include "Liquid/Renderer/API/Swapchain.h"

#include "DX11Common.h"

namespace Liquid {

	class DX11Swapchain : public Swapchain
	{
	public:
		DX11Swapchain(const SwapchainCreateInfo& createInfo);
		virtual ~DX11Swapchain();

		virtual void Resize(uint32 width, uint32 height, bool fullscreen) override;

		virtual void BeginFrame() const override;
		virtual void Present() const override;
		virtual void Clear(SwapchainBufferType buffer) override;

		virtual void SetVSync(bool enabled) override { m_VSync = enabled; }
		virtual bool IsVSyncEnabled() const override { return m_VSync; }
	private:
		SwapchainCreateInfo m_CreateInfo;
		uint32 m_Width = 0;
		uint32 m_Height = 0;
		bool m_VSync = false;

		DXRef<IDXGISwapChain> m_SwapChain;

		DXRef<ID3D11RenderTargetView> m_BackBuffer;
		DXRef<ID3D11Texture2D> m_DepthStencilBuffer;
		DXRef<ID3D11DepthStencilView> m_DepthStencilView;
	};

}