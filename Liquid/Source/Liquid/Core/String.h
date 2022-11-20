#pragma once

#include <string>

namespace Liquid {

	typedef std::string String;
	typedef std::wstring WString;
	typedef std::u16string U16String;
	typedef std::u32string U32String;

	namespace StringUtils {

		String FromWideString(const WString& string);
		WString ToWideString(const String& string);

		U16String ToUTF16String(const String& string);
		U32String ToUTF32String(const String& string);

	}

}