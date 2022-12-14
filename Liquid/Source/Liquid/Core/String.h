#pragma once

#include <string>

namespace Liquid {

	typedef std::string String;
	typedef std::wstring WString;
	typedef std::string_view StringView;
	typedef std::wstring_view WStringView;
	typedef std::u16string U16String;
	typedef std::u32string U32String;

	namespace StringUtils {

		String FromWideString(const WString& string);
		WString ToWideString(const String& string);

		U16String ToUTF16String(const String& string);
		U32String ToUTF32String(const String& string);

		String FormatBytes(size_t bytes);

		String ReplaceAll(String string, const std::string& from, const std::string& to);

		bool ContainsIgnoreCase(const String& string, const String& searchString);

		std::vector<String> SplitString(String s, const String& delimiter);
		String ToLowerString(String string);
		String ToUpperString(String string);
		String VectorToString(const std::vector<String>& strings);

	}

}