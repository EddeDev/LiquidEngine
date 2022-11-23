#pragma once

namespace Liquid {

	class ImGuiRenderer : public RefCounted
	{
	public:
		virtual ~ImGuiRenderer();

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

	private:
		static Ref<ImGuiRenderer> Create();
	};

}