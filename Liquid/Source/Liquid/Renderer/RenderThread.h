#pragma once

namespace Liquid {

	class RenderThreadQueue
	{
	public:
		static void Init(size_t initialBufferSize = 1024 * 1024 * 10);
		static void Shutdown();

		static void Reset();
		static void Flush();

		typedef void(*CommandFn)(void*);
		static uint8* Allocate(CommandFn function, size_t size);

		template<typename TSubmitInfo, bool TRelease, typename TLambda>
		static void Submit(TLambda&& lambda)
		{
#ifdef LQ_BUILD_DEBUG
			LQ_CHECK(s_IsInitialized.load());

			// TODO: Deferred release queue
			// TEMP
			if constexpr (!TRelease)
				LQ_CHECK(!s_Flushing.load());
			
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

		template<typename TSubmitInfo, typename TLambda>
		static void Submit(TLambda& lambda)
		{
			static_assert(sizeof(TLambda) == 0);
		}
	private:
		static std::atomic<bool> s_IsInitialized;
		static std::atomic<bool> s_Flushing;
	};


#define RT_SUBMIT(type) \
struct type##Name \
{ \
	static const char* GetName() { return #type; } \
}; \
RenderThreadQueue::Submit<type##Name, false>

#define RT_SUBMIT_RELEASE(type) \
struct type##Name \
{ \
	static const char* GetName() { return #type; } \
}; \
RenderThreadQueue::Submit<type##Name, true>

}