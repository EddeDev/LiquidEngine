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

		template<typename TSizeType = uint32>
		void Insert(const void* data, TSizeType size = 0)
		{
			LQ_CHECK(Data && data);

			memcpy(Data, data, static_cast<size_t>(size == 0 ? Size : size));
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

		template<typename T = uint8>
		T* GetData()
		{
			return (T*)Data;
		}

		template<typename T = uint8>
		const T* GetData() const
		{
			return (T*)Data;
		}

		uint32 GetSize() const { return Size; }
	};

}