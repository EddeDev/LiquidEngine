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

	String StringUtils::FormatBytes(size_t bytes)
	{
		constexpr size_t KB = 1024;     
		constexpr size_t MB = KB * 1024;
		constexpr size_t GB = MB * 1024;

		if (bytes >= GB)
			return fmt::format("{0} GB", bytes / GB);
		if (bytes >= MB)
			return fmt::format("{0} MB", bytes / MB);
		if (bytes >= KB)
			return fmt::format("{0} KB", bytes / KB);

		return fmt::format("{0} byte{1}", bytes, bytes != 1 ? "s" : "");
	}

}