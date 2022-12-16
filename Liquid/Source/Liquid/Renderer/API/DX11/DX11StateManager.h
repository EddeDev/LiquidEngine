#pragma once

#include "DX11Common.h"

#include "Liquid/Renderer/API/Shader.h"

// State manager (Bind/Unbind calls to DX11)

namespace Liquid {

	class DX11StateManager
	{
	public:
		static void BindShader(ShaderStage stage, ID3D11DeviceChild* shader);
		static void UnbindShader(ShaderStage stage);

		static void BindShaderResourceView(ShaderStage stage, ID3D11ShaderResourceView* shaderResourceView, uint32 slot = 0);
		static void UnbindShaderResourceView(ShaderStage stage, uint32 slot = 0);

		static void BindUnorderedAccessView(ID3D11UnorderedAccessView* unorderedAccessView, uint32 slot = 0);
		static void UnbindUnorderedAccessView(uint32 slot = 0);

		static void BindRenderTargetViews(const std::vector<ID3D11RenderTargetView*>& renderTargetViews);
		static void UnbindRenderTargetViews();
	};

}