#pragma once

#include "PrimitiveTypes.h"

namespace Liquid {

	struct DynamicBuffer
	{
	private:
		uint8* Data = nullptr;
		uint32 Size = 0;
	public:
		DynamicBuffer() = default;
		DynamicBuffer(const DynamicBuffer&) = default;
		DynamicBuffer(uint8* data, uint32 size)
			: Data(data), Size(size) {}

		void Allocate(uint32 size)
		{
			Release();

			Data = new uint8[size];
			Size = size;
		}

		void Insert(const void* data, uint32 size = 0)
		{
			if (!Data || !data)
				LQ_PLATFORM_BREAK();

			memcpy(Data, data, size == 0 ? Size : size);
		}

		void Release()
		{
			delete[] Data;
			Data = nullptr;

			Size = 0;
		}

		void FillWithZeroes()
		{
			memset(Data, 0, Size);
		}

		operator bool() { return Data != nullptr; }
		operator bool() const { return Data != nullptr; }

		uint8* Get() { return Data; }
		const uint8* Get() const { return Data; }
	};

}