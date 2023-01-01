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

	bool StringUtils::ContainsIgnoreCase(const String& string, const String& searchString)
	{
		return ToLowerString(string) == ToLowerString(searchString);
	}

	std::vector<Liquid::String> StringUtils::SplitString(String s, const String& delimiter)
	{
		size_t begin = 0, end, delimiterLength = delimiter.length();
		String token;
		std::vector<String> result;

		while ((end = s.find(delimiter, begin)) != String::npos) {
			token = s.substr(begin, end - begin);
			begin = end + delimiterLength;
			result.push_back(token);
		}

		result.push_back(s.substr(begin));
		return result;
	}

	String StringUtils::ToLowerString(String string)
	{
		std::transform(string.begin(), string.end(), string.begin(), [](uint8 c)
		{
			return std::tolower(c);
		});
		return string;
	}

	String StringUtils::ToUpperString(String string)
	{
		std::transform(string.begin(), string.end(), string.begin(), [](uint8 c)
		{
			return std::toupper(c);
		});
		return string;
	}

	String StringUtils::VectorToString(const std::vector<String>& strings)
	{
		std::stringstream ss;
		for (size_t i = 0; i < strings.size(); i++)
		{
			if (i > 0)
				ss << ", ";
			ss << strings[i];
		}
		return ss.str();
	}

}