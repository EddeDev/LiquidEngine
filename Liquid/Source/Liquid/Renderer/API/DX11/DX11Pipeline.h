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
		virtual void Unbind() const override;

		virtual void RT_Bind() const override;
		virtual void RT_Unbind() const override;

		virtual void Draw(uint32 vertexCount, uint32 startVertexLocation) const override;
		virtual void RT_Draw(uint32 vertexCount, uint32 startVertexLocation) const override;

		virtual void DrawIndexed(uint32 indexCount, uint32 startIndexLocation, int32 baseVertexLocation) const override;
		virtual void RT_DrawIndexed(uint32 indexCount, uint32 startIndexLocation, int32 baseVertexLocation) const override;
	private:
		GraphicsPipelineCreateInfo m_CreateInfo;

		ID3D11InputLayout* m_InputLayout = nullptr;
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_VertexElements;

		ID3D11RasterizerState* m_RasterizerState = nullptr;
		ID3D11DepthStencilState* m_DepthStencilState = nullptr;
	};

}