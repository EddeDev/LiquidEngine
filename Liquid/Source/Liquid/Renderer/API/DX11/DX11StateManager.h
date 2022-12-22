#pragma once

#include "DX11Common.h"

#include "Liquid/Renderer/API/Shader.h"

namespace Liquid {

	class DX11StateManager
	{
	public:
		static void BindShader(ShaderStage stage, ID3D11DeviceChild* shader);
		static void UnbindShader(ShaderStage stage);

		static void BindVertexBuffer(ID3D11Buffer* buffer, uint32 stride, uint32 offset = 0);
		static void UnbindVertexBuffer();

		static void BindIndexBuffer(ID3D11Buffer* buffer);
		static void UnbindIndexBuffer();

		static void BindInputLayout(ID3D11InputLayout* inputLayout);
		static void UnbindInputLayout();

		static void BindRasterizerState(ID3D11RasterizerState* rasterizerState);
		static void UnbindRasterizerState();

		static void BindDepthStencilState(ID3D11DepthStencilState* depthStencilState);
		static void UnbindDepthStencilState();

		static void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
		static void UnbindPrimitiveTopology();

		static void BindShaderResourceView(ShaderStage stage, ID3D11ShaderResourceView* shaderResourceView, uint32 slot = 0);
		static void UnbindShaderResourceView(ShaderStage stage, uint32 slot = 0);

		static void BindUnorderedAccessView(ID3D11UnorderedAccessView* unorderedAccessView, uint32 slot = 0);
		static void UnbindUnorderedAccessView(uint32 slot = 0);

		static void BindRenderTargetViews(const std::vector<ID3D11RenderTargetView*>& renderTargetViews);
		static void UnbindRenderTargetViews();
	};

}