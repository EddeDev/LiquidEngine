#include "LiquidPCH.h"
#include "RenderThread.h"

namespace Liquid {

	bool RenderThreadQueue::s_IsInitialized;
	bool RenderThreadQueue::s_IsInRenderingThread;

	struct RenderThreadQueueData
	{
		uint8* Buffer;
		uint8* BufferPtr;

		uint64 CommandCount;
	};

	RenderThreadQueueData* s_Data = nullptr;

	void RenderThreadQueue::Init(size_t initialBufferSize)
	{
		s_Data = new RenderThreadQueueData();
		memset(s_Data, 0, sizeof(RenderThreadQueueData));

		s_Data->Buffer = new uint8[initialBufferSize];
		s_Data->BufferPtr = s_Data->Buffer;

		s_IsInitialized = true;
	}

	void RenderThreadQueue::Shutdown()
	{
		delete s_Data->Buffer;
		delete s_Data;
		s_Data = nullptr;
	}

	void RenderThreadQueue::Reset()
	{
		s_Data->BufferPtr = s_Data->Buffer;
		s_Data->CommandCount = 0;
	}

	void RenderThreadQueue::Flush()
	{
		s_IsInRenderingThread = true;

		uint8* currentBufferPtr = s_Data->Buffer;
		for (size_t i = 0; i < s_Data->CommandCount; i++)
		{
			CommandFn function = *(CommandFn*)currentBufferPtr;
			currentBufferPtr += sizeof(CommandFn);

			size_t bufferSize = *(size_t*)currentBufferPtr;
			currentBufferPtr += sizeof(size_t);

			function(currentBufferPtr);
			currentBufferPtr += bufferSize;
		}

		Reset();
		s_IsInRenderingThread = false;
	}

	uint8* RenderThreadQueue::Allocate(CommandFn function, size_t size)
	{
		*(CommandFn*)s_Data->BufferPtr = function;
		s_Data->BufferPtr += sizeof(CommandFn);

		*(size_t*)s_Data->BufferPtr = size;
		s_Data->BufferPtr += sizeof(size_t);

		uint8* bufferPtr = s_Data->BufferPtr;
		s_Data->BufferPtr += size;

		s_Data->CommandCount++;
		return bufferPtr;
	}

}