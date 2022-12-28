#pragma once

#include "Liquid/Renderer/API/IndexBuffer.h"

#include "DX11Common.h"

namespace Liquid {

	class DX11IndexBuffer : public IndexBuffer
	{
	public:
		DX11IndexBuffer(const void* data, uint32 size, IndexBufferDataType dataType);
		DX11IndexBuffer(uint32 size, IndexBufferDataType dataType);
		virtual ~DX11IndexBuffer();

		virtual void Bind() const override;
		virtual void RT_Bind() const override;

		virtual uint32 GetSize() const override { return m_Storage.GetSize(); }
		virtual uint32 GetCount() const override { return m_Storage.GetSize() / IndexBufferUtils::IndexBufferDataTypeSize(m_DataType); }

		virtual IndexBufferDataType GetDataType() const override { return m_DataType; }

		ID3D11Buffer* GetBuffer() const { return m_Buffer; }
	private:
		void Invalidate();
		void RT_Invalidate();
	private:
		IndexBufferDataType m_DataType;
		DynamicBuffer m_Storage;
		ID3D11Buffer* m_Buffer = nullptr;
	};

}