#pragma once

namespace Liquid {

	namespace Enum {

#define UNDERLYING_TYPE(type) __underlying_type(type)
#define DEFINE_ENUM_OPERATORS(TEnum) \
		inline           TEnum& operator|=(TEnum& lhs, TEnum rhs) { return lhs = (TEnum)((UNDERLYING_TYPE(TEnum))lhs | (UNDERLYING_TYPE(TEnum))rhs); } \
		inline           TEnum& operator&=(TEnum& lhs, TEnum rhs) { return lhs = (TEnum)((UNDERLYING_TYPE(TEnum))lhs & (UNDERLYING_TYPE(TEnum))rhs); } \
		inline           TEnum& operator^=(TEnum& lhs, TEnum rhs) { return lhs = (TEnum)((UNDERLYING_TYPE(TEnum))lhs ^ (UNDERLYING_TYPE(TEnum))rhs); } \
		inline constexpr TEnum  operator| (TEnum  lhs, TEnum rhs) { return (TEnum)((UNDERLYING_TYPE(TEnum))lhs | (UNDERLYING_TYPE(TEnum))rhs); } \
		inline constexpr TEnum  operator& (TEnum  lhs, TEnum rhs) { return (TEnum)((UNDERLYING_TYPE(TEnum))lhs & (UNDERLYING_TYPE(TEnum))rhs); } \
		inline constexpr TEnum  operator^ (TEnum  lhs, TEnum rhs) { return (TEnum)((UNDERLYING_TYPE(TEnum))lhs ^ (UNDERLYING_TYPE(TEnum))rhs); } \
		inline constexpr bool   operator! (TEnum  e)              { return !(UNDERLYING_TYPE(TEnum))e; } \
		inline constexpr TEnum  operator~ (TEnum  e)              { return (TEnum)~(UNDERLYING_TYPE(TEnum))e; }

		template<typename TEnum>
		constexpr bool HasAllFlags(TEnum flags, TEnum contains)
		{
			return (((UNDERLYING_TYPE(TEnum))flags) & (UNDERLYING_TYPE(TEnum))contains) == ((UNDERLYING_TYPE(TEnum))contains);
		}

		template<typename TEnum>
		constexpr bool HasAnyFlags(TEnum flags, TEnum contains)
		{
			return (((UNDERLYING_TYPE(TEnum))flags) & (UNDERLYING_TYPE(TEnum))contains) != 0;
		}

		template<typename TEnum>
		void AddFlags(TEnum& flags, TEnum& flagsToAdd)
		{
			flags |= flagsToAdd;
		}

		template<typename TEnum>
		void RemoveFlags(TEnum& flags, TEnum& flagsToRemove)
		{
			flags &= ~flagsToRemove;
		}

	}

}