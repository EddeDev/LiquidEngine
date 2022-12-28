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

		virtual void Bind() const override;
		virtual void RT_Bind() const override;

		virtual void DrawIndexed(Ref<IndexBuffer> indexBuffer, uint32 baseVertexIndex, uint32 vertexCount, uint32 startIndex, uint32 primitiveCount) const override;
		virtual void RT_DrawIndexed(Ref<IndexBuffer> indexBuffer, uint32 baseVertexIndex, uint32 vertexCount, uint32 startIndex, uint32 primitiveCount) const override;
	private:
		GraphicsPipelineCreateInfo m_CreateInfo;

		ID3D11InputLayout* m_InputLayout = nullptr;
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_VertexElements;

		ID3D11RasterizerState* m_RasterizerState = nullptr;
		ID3D11DepthStencilState* m_DepthStencilState = nullptr;
	};

}