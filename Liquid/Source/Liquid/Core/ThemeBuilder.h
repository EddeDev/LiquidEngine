#pragma once

namespace Liquid {

	class ThemeBuilder
	{
	public:
		ThemeBuilder() = default;

		void Render();
	private:
		std::stringstream m_CodeBuffer;

		bool m_ShowOutput = false;
		bool m_NeedsUpdate = false;
	};

}