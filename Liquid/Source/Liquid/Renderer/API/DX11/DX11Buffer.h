#pragma once

#include "Liquid/Renderer/API/Buffer.h"

#include "DX11Common.h"

namespace Liquid {

	class DX11Buffer : public Buffer
	{
	public:
		DX11Buffer(const void* data, uint32 size, BufferUsage usage);
		DX11Buffer(uint32 size, BufferUsage usage);
		virtual ~DX11Buffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void RT_Bind() const override;
		virtual void RT_Unbind() const override;

		virtual uint32 GetSize() const override { return m_Storage.GetSize(); }

		ID3D11Buffer* GetBuffer() const { return m_Buffer; }

		static ID3D11Buffer* GetCurrentlyBoundVertexBuffer() { return s_CurrentlyBoundVertexBuffer; }
		static ID3D11Buffer* GetCurrentlyBoundIndexBuffer() { return s_CurrentlyBoundIndexBuffer; }
	private:
		void Invalidate();
		void RT_Invalidate();
	private:
		inline static ID3D11Buffer* s_CurrentlyBoundVertexBuffer = nullptr;
		inline static ID3D11Buffer* s_CurrentlyBoundIndexBuffer = nullptr;

		BufferUsage m_Usage = BufferUsage::None;
		DynamicBuffer m_Storage;
		ID3D11Buffer* m_Buffer = nullptr;
		uint32 m_Stride = 0;
	};

}