#pragma once

namespace Liquid {

	class ImGuiImplementation : public RefCounted
	{
	public:
		virtual ~ImGuiImplementation() {}

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		static Ref<ImGuiImplementation> Create();
	};

}