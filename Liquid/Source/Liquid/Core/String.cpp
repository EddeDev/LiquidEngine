#include "LiquidPCH.h"
#include "String.h"

#include <codecvt>

namespace Liquid {

	String StringUtils::FromWideString(const WString& string)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.to_bytes(string);
	}

	WString StringUtils::ToWideString(const String& string)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return conv.from_bytes(string);
	}

	U16String StringUtils::ToUTF16String(const String& string)
	{
		std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
		return conv.from_bytes(string);
	}

	U32String StringUtils::ToUTF32String(const String& string)
	{
		std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
		return conv.from_bytes(string);
	}

}