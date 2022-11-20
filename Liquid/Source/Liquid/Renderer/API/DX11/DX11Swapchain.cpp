#include "LiquidPCH.h"
#include "DX11Swapchain.h"

#include "DX11Context.h"

namespace Liquid {

	namespace Utils {

		static DXGI_FORMAT DX11PixelFormat(PixelFormat format)
		{
			switch (format)
			{
			case PixelFormat::RGBA:                   return DXGI_FORMAT_R8G8B8A8_UNORM;
			case PixelFormat::RGBA16F:                return DXGI_FORMAT_R16G16B16A16_FLOAT;
			case PixelFormat::RGBA32F:                return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case PixelFormat::DEPTH24_STENCIL8:       return DXGI_FORMAT_D24_UNORM_S8_UINT;
			case PixelFormat::DEPTH32F_STENCIL8_UINT: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			case PixelFormat::DEPTH32F:               return DXGI_FORMAT_D32_FLOAT;
			}
			LQ_VERIFY(false, "Unknown pixel format");
			return DXGI_FORMAT_UNKNOWN;
		}

	}

	DX11Swapchain::DX11Swapchain(const SwapchainCreateInfo& createInfo)
		: m_CreateInfo(createInfo)
	{
		Ref<DX11Context> context = GraphicsContext::Get<DX11Context>();
		DXRef<ID3D11Device> device = context->GetDevice();

		DXGI_RATIONAL refreshRate;
		refreshRate.Numerator = 60;
		refreshRate.Denominator = 1;

		DXGI_MODE_DESC bufferDesc = {};
		bufferDesc.Width = createInfo.InitialWidth;
		bufferDesc.Height = createInfo.InitialHeight;
		bufferDesc.RefreshRate = refreshRate;
		bufferDesc.Format = Utils::DX11PixelFormat(createInfo.ColorFormat);

		uint32 msaaQuality;
		DX_CHECK(device->CheckMultisampleQualityLevels(bufferDesc.Format, createInfo.SampleCount, &msaaQuality));

		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = createInfo.SampleCount;
		sampleDesc.Quality = msaaQuality - 1;

		DXGI_SWAP_CHAIN_DESC swapchainDesc;
		swapchainDesc.BufferDesc = bufferDesc;
		swapchainDesc.SampleDesc = sampleDesc;
		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchainDesc.BufferCount = createInfo.BufferCount;
		swapchainDesc.OutputWindow = static_cast<HWND>(createInfo.WindowHandle);
		swapchainDesc.Windowed = true;
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		DXRef<IDXGIDevice> dxgiDevice;
		DXRef<IDXGIAdapter> dxgiAdapter;
		DXRef<IDXGIFactory> dxgiFactory;
		DX_CHECK(device->QueryInterface(DX_RIID(IDXGIDevice), &dxgiDevice));
		DX_CHECK(dxgiDevice->GetParent(DX_RIID(IDXGIAdapter), &dxgiAdapter));
		DX_CHECK(dxgiAdapter->GetParent(DX_RIID(IDXGIFactory), &dxgiFactory));

		DX_CHECK(dxgiFactory->CreateSwapChain(device.Get(), &swapchainDesc, &m_SwapChain));

		Resize(createInfo.InitialWidth, createInfo.InitialHeight);
	}

	void DX11Swapchain::Resize(uint32 width, uint32 height)
	{
		if (width == 0 || height == 0)
			return;

		Ref<DX11Context> context = GraphicsContext::Get<DX11Context>();
		DXRef<ID3D11Device> device = context->GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = context->GetDeviceContext();

		m_BackBuffer.Reset();
		m_DepthStencilView.Reset();
		m_DepthStencilBuffer.Reset();

		DX_CHECK(m_SwapChain->ResizeBuffers(m_CreateInfo.BufferCount, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

		DXRef<ID3D11Texture2D> backBuffer;
		DX_CHECK(m_SwapChain->GetBuffer(0, DX_RIID(ID3D11Texture2D), &backBuffer));
		DX_CHECK(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_BackBuffer));

		uint32 msaaQuality;
		DX_CHECK(device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_CreateInfo.SampleCount, &msaaQuality));

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = Utils::DX11PixelFormat(m_CreateInfo.DepthFormat);
		depthStencilDesc.SampleDesc.Count = m_CreateInfo.SampleCount;
		depthStencilDesc.SampleDesc.Quality = msaaQuality - 1;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		DX_CHECK(device->CreateTexture2D(&depthStencilDesc, nullptr, &m_DepthStencilBuffer));
		DX_CHECK(device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), nullptr, &m_DepthStencilView));
		deviceContext->OMSetRenderTargets(1, m_BackBuffer.GetAddressOf(), m_DepthStencilView.Get());

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		deviceContext->RSSetViewports(1, &viewport);
	}

	void DX11Swapchain::Present() const
	{
		Ref<DX11Context> context = GraphicsContext::Get<DX11Context>();
		DXRef<ID3D11Device> device = context->GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = context->GetDeviceContext();

		static float red, green, blue;
		static float colormodr = 1, colormodg = 1, colormodb = 1;
		red += colormodr * 0.005f;
		green += colormodg * 0.002f;
		blue += colormodb * 0.001f;
		if (red >= 1.0f || red <= 0.0f)
			colormodr *= -1;
		if (green >= 1.0f || green <= 0.0f)
			colormodg *= -1;
		if (blue >= 1.0f || blue <= 0.0f)
			colormodb *= -1;

		const float color[4] = { red, green, blue, 1.0f };
		deviceContext->ClearRenderTargetView(m_BackBuffer.Get(), color);

		uint32 clearFlags = D3D11_CLEAR_DEPTH;
		if (PixelFormatUtils::IsStencilFormat(m_CreateInfo.DepthFormat))
			clearFlags |= D3D11_CLEAR_STENCIL;

		const float depth = 1.0f;
		const uint8 stencil = 0;
		deviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), clearFlags, depth, stencil);

		m_SwapChain->Present(1, 0);
	}

}