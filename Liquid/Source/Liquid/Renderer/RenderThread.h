#pragma once

#include "Liquid/Core/Application.h"

namespace Liquid {

	class RenderThreadQueue
	{
	public:
		static void Init(size_t initialBufferSize);
		static void Shutdown();

		static void Reset();
		static void Flush();

		typedef void(*CommandFn)(void*);
		static uint8* Allocate(CommandFn function, size_t size);

		static bool IsInRenderingThread()
		{
			auto currentThreadID = std::this_thread::get_id();
			auto renderThreadID = Application::GetRenderThread()->GetThreadID();

			return currentThreadID == renderThreadID;
		}

		template<typename TName, typename TLambda>
		static void Submit(TLambda&& lambda)
		{
#ifdef LQ_BUILD_DEBUG
			if (!s_IsInitialized.load())
				LQ_PLATFORM_BREAK();

			if (s_Executing.load())
				LQ_PLATFORM_BREAK();
#endif

#if 0
			if (IsInRenderingThread())
			{
				lambda();
			}
			else
#endif
			{
				CommandFn commandFn = [](void* bufferPtr)
				{
					TLambda* lambdaPtr = static_cast<TLambda*>(bufferPtr);
					(*lambdaPtr)();

					// Explicitly call destructor
					lambdaPtr->~TLambda();
				};

				uint8* bufferPtr = Allocate(commandFn, sizeof(lambda));
				new (bufferPtr) TLambda(std::forward<TLambda>(lambda));
			}
		}
	private:
		static std::atomic<bool> s_IsInitialized;
		static std::atomic<bool> s_Executing;
	};


#define RT_SUBMIT(type) \
struct type##Name \
{ \
	static const char* GetName() { return #type; } \
}; \
RenderThreadQueue::Submit<type##Name>

}