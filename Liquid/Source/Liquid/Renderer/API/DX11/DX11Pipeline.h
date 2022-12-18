#pragma once

#include "Liquid/Renderer/API/GraphicsPipeline.h"

#include "DX11Common.h"

namespace Liquid {

	class DX11Pipeline : public GraphicsPipeline
	{
	public:
		DX11Pipeline(const GraphicsPipelineCreateInfo& createInfo);
		virtual ~DX11Pipeline();

		virtual void Invalidate() override;
		virtual void RT_Invalidate() override;

		virtual void DrawIndexed(Ref<Buffer> vertexBuffer, Ref<Buffer> indexBuffer) const override;
	private:
		GraphicsPipelineCreateInfo m_CreateInfo;

		ID3D11InputLayout* m_InputLayout = nullptr;

		ID3D11RasterizerState* m_RasterizerState = nullptr;
		ID3D11DepthStencilState* m_DepthStencilState = nullptr;
	};

}