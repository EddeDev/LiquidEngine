#pragma once

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

		template<typename TName, typename TLambda>
		static void Submit(TLambda&& lambda)
		{
#ifdef LQ_BUILD_DEBUG
			if (!s_IsInitialized)
				LQ_PLATFORM_BREAK();
#endif

			if (s_IsInRenderingThread)
			{
				LQ_WARNING_ARGS("({0}) RenderThreadQueue::Submit was called on the render thread", TName::GetName());
				lambda();
			}
			else
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
		static bool s_IsInitialized;
		static bool s_IsInRenderingThread;
	};


#define RT_SUBMIT(type) \
struct type##Name \
{ \
	static const char* GetName() { return #type; } \
}; \
RenderThreadQueue::Submit<type##Name>

}