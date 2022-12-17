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

		virtual uint32 GetSize() const override { return m_Storage.GetSize(); }

		ID3D11Buffer* GetBuffer() const { return m_Buffer; }
	private:
		BufferUsage m_Usage = BufferUsage::None;
		DynamicBuffer m_Storage;
		ID3D11Buffer* m_Buffer = nullptr;
	};

}