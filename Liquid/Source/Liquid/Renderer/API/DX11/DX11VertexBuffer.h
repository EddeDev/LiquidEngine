#pragma once

#include "Liquid/Renderer/API/VertexBuffer.h"

#include "DX11Common.h"

namespace Liquid {

	class DX11VertexBuffer : public VertexBuffer
	{
	public:
		DX11VertexBuffer(const void* data, uint32 size, VertexBufferUsage usage);
		DX11VertexBuffer(uint32 size, VertexBufferUsage usage);
		virtual ~DX11VertexBuffer();

		virtual void Bind() const override;
		virtual void RT_Bind() const override;

		virtual uint32 GetSize() const override { return m_Storage.GetSize(); }

		ID3D11Buffer* GetBuffer() const { return m_Buffer; }

		static ID3D11Buffer* GetCurrentlyBoundBuffer() { return s_CurrentlyBoundBuffer; }
	private:
		void Invalidate();
		void RT_Invalidate();
	private:
		inline static ID3D11Buffer* s_CurrentlyBoundBuffer = nullptr;

		VertexBufferUsage m_Usage = VertexBufferUsage::None;
		DynamicBuffer m_Storage;
		ID3D11Buffer* m_Buffer = nullptr;
		uint32 m_Stride = 0;
	};

}