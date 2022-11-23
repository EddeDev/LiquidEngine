#include "LiquidPCH.h"
#include "DX11Swapchain.h"

#include "DX11Device.h"

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
		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();

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
		swapchainDesc.Windowed = !createInfo.InitialFullscreenState;
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		if (createInfo.AllowTearing)
			swapchainDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

		DXRef<IDXGIDevice> dxgiDevice;
		DXRef<IDXGIAdapter> dxgiAdapter;
		DXRef<IDXGIFactory> dxgiFactory;
		DX_CHECK(device->QueryInterface(DX_RIID(IDXGIDevice), &dxgiDevice));
		DX_CHECK(dxgiDevice->GetParent(DX_RIID(IDXGIAdapter), &dxgiAdapter));
		DX_CHECK(dxgiAdapter->GetParent(DX_RIID(IDXGIFactory), &dxgiFactory));

		DX_CHECK(dxgiFactory->CreateSwapChain(device.Get(), &swapchainDesc, &m_SwapChain));
		DX_CHECK(dxgiFactory->MakeWindowAssociation(swapchainDesc.OutputWindow, DXGI_MWA_NO_WINDOW_CHANGES));

		Resize(createInfo.InitialWidth, createInfo.InitialHeight, createInfo.InitialFullscreenState);
	}

	DX11Swapchain::~DX11Swapchain()
	{
		m_SwapChain->SetFullscreenState(false, nullptr);
	}

	void DX11Swapchain::Resize(uint32 width, uint32 height, bool fullscreen)
	{
		if (width == 0 || height == 0)
		{
			LQ_VERIFY(false, "Attempted to resize swapchain to [{0}, {1}]", width, height);
			return;
		}

		LQ_TRACE_ARGS("DX11Swapchain::Resize({0}, {1}, {2})", width, height, fullscreen);

		DXRef<ID3D11Device> device = DX11Device::Get().GetDevice();
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		deviceContext->ClearState();

		m_BackBuffer.Reset();
		m_DepthStencilView.Reset();
		m_DepthStencilBuffer.Reset();

		uint32 flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		if (m_CreateInfo.AllowTearing)
			flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

		DX_CHECK(m_SwapChain->ResizeBuffers(0, width, height, Utils::DX11PixelFormat(m_CreateInfo.ColorFormat), flags));

		if (fullscreen)
		{
			DXGI_RATIONAL refreshRate;
			refreshRate.Numerator = 60;
			refreshRate.Denominator = 1;

			DXGI_MODE_DESC bufferDesc = {};
			bufferDesc.Width = width;
			bufferDesc.Height = height;
			bufferDesc.RefreshRate = refreshRate;
			bufferDesc.Format = Utils::DX11PixelFormat(m_CreateInfo.ColorFormat);

			if (FAILED(m_SwapChain->ResizeTarget(&bufferDesc)))
			{
				DX_CHECK(m_SwapChain->SetFullscreenState(fullscreen, nullptr));
				DX_CHECK(m_SwapChain->ResizeBuffers(0, width, height, Utils::DX11PixelFormat(m_CreateInfo.ColorFormat), flags));
			}
		}

		DXRef<ID3D11Texture2D> backBuffer;
		DX_CHECK(m_SwapChain->GetBuffer(0, DX_RIID(ID3D11Texture2D), &backBuffer));
		DX_CHECK(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_BackBuffer));

		uint32 msaaQuality;
		DX_CHECK(device->CheckMultisampleQualityLevels(Utils::DX11PixelFormat(m_CreateInfo.ColorFormat), m_CreateInfo.SampleCount, &msaaQuality));

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

	void DX11Swapchain::BeginFrame() const
	{
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();
		deviceContext->OMSetRenderTargets(1, m_BackBuffer.GetAddressOf(), m_DepthStencilView.Get());
	}

	void DX11Swapchain::Present() const
	{
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		int32 fullscreen;
		DX_CHECK(m_SwapChain->GetFullscreenState(&fullscreen, nullptr));

		uint32 flags = 0;
		if (m_CreateInfo.AllowTearing && !m_VSync && !fullscreen)
			flags |= DXGI_PRESENT_ALLOW_TEARING;

		HRESULT result = m_SwapChain->Present(m_VSync ? 1 : 0, flags);
		if (FAILED(result))
		{
			DXGI_SWAP_CHAIN_DESC desc;
			if (!FAILED(m_SwapChain->GetDesc(&desc)))
			{
				LQ_ERROR_ARGS("Swapchain Desc:");
				LQ_ERROR_ARGS("  BufferDesc.Width = {0}", desc.BufferDesc.Width);
				LQ_ERROR_ARGS("  BufferDesc.Height = {0}", desc.BufferDesc.Height);
				LQ_ERROR_ARGS("  BufferDesc.RefreshRate.Numerator = {0}", desc.BufferDesc.RefreshRate.Numerator);
				LQ_ERROR_ARGS("  BufferDesc.RefreshRate.Denominator = {0}", desc.BufferDesc.RefreshRate.Denominator);
				LQ_ERROR_ARGS("  BufferDesc.Format = {0}", desc.BufferDesc.Format);
				LQ_ERROR_ARGS("  BufferDesc.ScanlineOrdering = {0}", desc.BufferDesc.ScanlineOrdering);
				LQ_ERROR_ARGS("  BufferDesc.Scaling = {0}", desc.BufferDesc.Scaling);
				LQ_ERROR_ARGS("  SampleDesc.Count = {0}", desc.SampleDesc.Count);
				LQ_ERROR_ARGS("  SampleDesc.Quality = {0}", desc.SampleDesc.Quality);
				LQ_ERROR_ARGS("  BufferUsage = {0}", desc.BufferUsage);
				LQ_ERROR_ARGS("  BufferCount = {0}", desc.BufferCount);
				LQ_ERROR_ARGS("  OutputWindow = {0}", static_cast<void*>(desc.OutputWindow));
				LQ_ERROR_ARGS("  Windowed = {0}", desc.Windowed ? "true" : "false");
				LQ_ERROR_ARGS("  SwapEffect = {0}", desc.SwapEffect);
				LQ_ERROR_ARGS("  Flags = {0}", desc.Flags);
			}
			
			DX_CHECK(result);
		}

		deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}

	void DX11Swapchain::Clear(uint32 buffer)
	{
		DXRef<ID3D11DeviceContext> deviceContext = DX11Device::Get().GetDeviceContext();

		if (buffer & BUFFER_COLOR)
		{
			const float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			deviceContext->ClearRenderTargetView(m_BackBuffer.Get(), color);
		}

		uint32 clearFlags = 0;
		if (buffer & BUFFER_DEPTH)
			clearFlags |= D3D11_CLEAR_DEPTH;
		if (buffer & BUFFER_STENCIL)
			clearFlags |= D3D11_CLEAR_STENCIL;

		if (clearFlags)
			deviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), clearFlags, 1.0f, 0);
	}

}