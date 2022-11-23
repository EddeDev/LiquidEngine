#pragma once

namespace Liquid {

	class ImGuiRenderer : public RefCounted
	{
	public:
		virtual ~ImGuiRenderer();

		void BeginFrame();
		void EndFrame();

		static Ref<ImGuiRenderer> Create();
	protected:
		virtual void Init() = 0;
		virtual void NewFrame() = 0;
		virtual void Render() = 0;
	private:
		void InitContext();
	};

}