//-----------------------------------------------------------------------------------------
// SPDX-License-Identifier: MIT
// conjure_enum (header only)
// Copyright (C) 2024 Fix8 Market Technologies Pty Ltd
//   by David L. Dight
// see https://github.com/fix8mt/conjure_enum
//
// Lightweight header-only C++20 enum reflection
//
//   Parts based on magic_enum <https://github.com/Neargye/magic_enum>
//   Copyright (c) 2019 - 2024 Daniil Goncharov <neargye@gmail.com>.
//
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice (including the next paragraph)
// shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//----------------------------------------------------------------------------------------
// clang
// static const char *FIX8::conjure_enum<component>::epeek() [T = component, e = component::path] // valid
// 																								  |<--				-->|
// static const char *FIX8::conjure_enum<component>::epeek() [T = component, e = (component)100] // invalid
//																									  |<--           -->|
// static const char *FIX8::conjure_enum<component>::tpeek() [T = component]
//																				  |<--		-->|
// gcc
// static consteval const char* FIX8::conjure_enum<T>::epeek() [with T e = component::path; T = component] // valid
//																							  |<--				-->|
// static consteval const char* FIX8::conjure_enum<T>::epeek() [with T e = (component)100; T = component] // invalid
//																							  |<--           -->|
// static consteval const char* FIX8::conjure_enum<T>::tpeek() [with T = component]
//																							|<--		 -->|
// msvc
// const char *__cdecl FIX8::conjure_enum<enum numbers>::epeek<numbers::two>(void) noexcept			// valid
//																					|<--		-->|
// const char *__cdecl FIX8::conjure_enum<enum numbers>::epeek<(enum numbers)0xa>(void) noexcept	// invalid
//																			|<--		 		 -->|
// const char *__cdecl FIX8::conjure_enum<enum numbers>::tpeek(void) noexcept
//														|<--   	-->|
//----------------------------------------------------------------------------------------
#ifndef FIX8_CONJURE_ENUM_HPP_
#define FIX8_CONJURE_ENUM_HPP_

//----------------------------------------------------------------------------------------
#include <source_location>
#include <algorithm>
#include <string_view>
#include <tuple>
#include <concepts>
#include <optional>
#include <functional>
#include <ostream>
#include <cstddef>
#include <exception>
#include <bitset>
#include <bit>
#include <array>

//-----------------------------------------------------------------------------------------
namespace FIX8 {

//-----------------------------------------------------------------------------------------
#if not defined ENUM_MIN_VALUE
# define ENUM_MIN_VALUE -128
#endif
#if not defined ENUM_MAX_VALUE
# define ENUM_MAX_VALUE 127
#endif

//-----------------------------------------------------------------------------------------
template<std::size_t N>
class fixed_string final
{
	std::array<char, N + 1> _buff;
	template<char... C>
	constexpr fixed_string(std::string_view sv, std::integer_sequence<char, C...>) noexcept : _buff{sv[C]..., 0} {}

public:
	constexpr fixed_string(std::string_view sv) noexcept : fixed_string{sv.data(), std::make_integer_sequence<char, N>{}} {}
	constexpr fixed_string() = delete;
	constexpr std::string_view get() const noexcept { return { _buff.data(), N }; }
	constexpr operator std::string_view() const noexcept { return get(); }
	constexpr std::size_t size() const noexcept { return _buff.size(); }
	friend std::ostream& operator<<(std::ostream& os, const fixed_string& what) { return os << what.get(); }
};

//-----------------------------------------------------------------------------------------
template<typename T>
requires std::is_enum_v<T>
class conjure_enum final
{
	static constexpr int enum_min_value{ENUM_MIN_VALUE}, enum_max_value{ENUM_MAX_VALUE};
	static_assert(enum_max_value > enum_min_value, "ENUM_MAX_VALUE must be greater than ENUM_MIN_VALUE");

	static constexpr auto _specifics
	{
      std::to_array<std::tuple<std::string_view, char>>
      ({
#if defined __clang__
			{ "e = ", ']' }, { "T = ", ']' },
#elif defined __GNUC__
			{ "e = ", ';' }, { "T = ", ']' },
#elif defined _MSC_VER
			{ "epeek<", '>' }, { "enum ", '>' },
#else
# error "compiler not supported"
#endif
      })
   };
	template<int N, int V> // can't have constexpr decompositions! (but why?)
	static constexpr auto _gpos() noexcept { return std::get<N>(_specifics[V]); }

	template<T e>
	static constexpr auto _enum_name() noexcept
	{
		constexpr auto result { _get_name<e>() };
		return fixed_string<result.size()>(result);
	}

public:
	using enum_tuple = std::tuple<T, std::string_view>;
	using scoped_tuple = std::tuple<std::string_view, std::string_view>;

	template<T e>
	static constexpr auto enum_name_v { _enum_name<e>() };

	static consteval const char *tpeek() noexcept { return std::source_location::current().function_name(); }
	template<T e>
	static consteval const char *epeek() noexcept { return std::source_location::current().function_name(); }

private:
	template<std::size_t... I>
	static constexpr auto _entries(std::index_sequence<I...>) noexcept
	{
		return std::array<enum_tuple, sizeof...(I)>{{{ enum_values[I], enum_name_v<enum_values[I]>}...}};
	}

	template<std::size_t... I>
	static constexpr auto _scoped_entries(std::index_sequence<I...>) noexcept
	{
		std::array<scoped_tuple, sizeof...(I)> tmp{{{ remove_scope(enum_name_v<enum_values[I]>), enum_name_v<enum_values[I]>}...}};
		std::sort(tmp.begin(), tmp.end(), scoped_comp);
		return tmp;
	}

	static constexpr auto _entries_sorted() noexcept
	{
		auto tmp { enum_entries };
		std::sort(tmp.begin(), tmp.end(), tuple_comp_rev);
		return tmp;
	}

	template< std::size_t... I>
	static constexpr auto _names(std::index_sequence<I...>) noexcept
	{
		return std::array<std::string_view, sizeof...(I)>{{{ enum_name_v<enum_values[I]>}...}};
	}

	template<std::size_t... I>
	static constexpr auto _values(std::index_sequence<I...>) noexcept
	{
		constexpr std::array<bool, sizeof...(I)> valid { is_valid<static_cast<T>(enum_min_value + I)>()... };
		constexpr auto num_valid { std::count_if(valid.cbegin(), valid.cend(), [](bool val) noexcept { return val; }) };
		static_assert(num_valid > 0, "conjure_enum requires non-empty enum");
		std::array<T, num_valid> vals{};
		for(std::size_t offset{}, nn{}; nn < num_valid; ++offset)
			if (valid[offset])
				vals[nn++] = static_cast<T>(enum_min_value + offset);
		return vals;
	}

	template<T e>
	static constexpr std::string_view _get_name() noexcept
	{
		constexpr std::string_view from{epeek<e>()};
		if (constexpr auto ep { from.rfind(_gpos<0,0>()) }; ep != std::string_view::npos && from[ep + _gpos<0,0>().size()] != '(')
		{
			constexpr std::string_view result { from.substr(ep + _gpos<0,0>().size()) };
			if (constexpr auto lc { result.find_first_of(_gpos<1,0>()) }; lc != std::string_view::npos)
				return result.substr(0, lc);
		}
		return {};
	}

	static constexpr bool value_comp(const T& pl, const T& pr) noexcept
	{
		return static_cast<int>(pl) < static_cast<int>(pr);
	}
	static constexpr bool tuple_comp(const enum_tuple& pl, const enum_tuple& pr) noexcept
	{
		return static_cast<int>(std::get<T>(pl)) < static_cast<int>(std::get<T>(pr));
	}
	static constexpr bool tuple_comp_rev(const enum_tuple& pl, const enum_tuple& pr) noexcept
	{
		return std::get<std::string_view>(pl) < std::get<std::string_view>(pr);
	}
	static constexpr bool scoped_comp(const scoped_tuple& pl, const scoped_tuple& pr) noexcept
	{
		return std::get<0>(pl) < std::get<0>(pr);
	}

public:
	conjure_enum() = delete;

	template<T e>
	static constexpr std::string_view enum_to_string() noexcept { return _get_name<e>(); }

	static constexpr std::string_view enum_type() noexcept
	{
		constexpr std::string_view from{tpeek()};
		if (constexpr auto ep { from.rfind(_gpos<0,1>()) }; ep != std::string_view::npos)
		{
			constexpr std::string_view result { from.substr(ep + _gpos<0,1>().size()) };
			if (constexpr auto lc { result.find_first_of(_gpos<1,1>()) }; lc != std::string_view::npos)
				return result.substr(0, lc);
		}
		return {};
	}

	struct is_scoped : std::integral_constant<bool, requires
		{ requires !std::is_convertible_v<T, std::underlying_type_t<T>>; }>
	{};

	template<T e>
	static constexpr bool is_valid() noexcept { return !_get_name<e>().empty(); }

	static constexpr auto _values() noexcept
	{
		return _values(std::make_index_sequence<enum_max_value - enum_min_value + 1>({}));
	}

	static constexpr auto count() noexcept { return enum_values.size(); }

	static constexpr std::string_view remove_scope(std::string_view what) noexcept
	{
		if constexpr (is_scoped())
		{
			if (const auto lc { what.find_last_of(':') }; lc != std::string_view::npos)
				return what.substr(lc + 1);
		}
		return what;
	}

	static constexpr std::string_view add_scope(std::string_view what) noexcept
	{
		if constexpr (is_scoped())
		{
			if (const auto result { std::equal_range(enum_scoped_entries.cbegin(),
				enum_scoped_entries.cend(), scoped_tuple(what, std::string_view()), scoped_comp) };
					result.first != result.second)
						return std::get<1>(*result.first);
		}
		return what;
	}

	static constexpr bool has_scope(std::string_view what) noexcept
	{
		if constexpr (is_scoped())
			return enum_contains(what);
		else
			return false;
	}

	static constexpr auto cbegin() noexcept { return enum_entries.cbegin(); }
	static constexpr auto cend() noexcept { return enum_entries.cend(); }
	static constexpr auto crbegin() noexcept { return enum_entries.crbegin(); }
	static constexpr auto crend() noexcept { return enum_entries.crend(); }
	static constexpr auto front() noexcept { return *enum_entries.cbegin(); }
	static constexpr auto back() noexcept { return *std::prev(enum_entries.cend()); }

	static constexpr std::optional<T> int_to_enum(int value) noexcept
	{
		if (const auto result { std::equal_range(enum_values.cbegin(), enum_values.cend(), static_cast<T>(value), value_comp) };
			result.first != result.second)
				return *result.first;
		return {};
	}
	static constexpr bool enum_contains(T value) noexcept
	{
		const auto result { std::equal_range(enum_values.cbegin(), enum_values.cend(), value, value_comp) };
		return result.first != result.second;
   }
	static constexpr bool enum_contains(std::string_view str) noexcept
	{
		const auto result { std::equal_range(enum_entries_sorted.cbegin(), enum_entries_sorted.cend(), enum_tuple(T{}, str), tuple_comp_rev) };
		return result.first != result.second;
	}
	static constexpr std::string_view enum_to_string(T value, bool noscope=false) noexcept
	{
		if (const auto result { std::equal_range(enum_entries.cbegin(), enum_entries.cend(), enum_tuple(value, std::string_view()), tuple_comp) };
			result.first != result.second)
				return noscope ? remove_scope(std::get<std::string_view>(*result.first)) : std::get<std::string_view>(*result.first);
		return {};
	}
	static constexpr std::optional<T> string_to_enum(std::string_view str) noexcept
	{
		if (const auto result { std::equal_range(enum_entries_sorted.cbegin(), enum_entries_sorted.cend(), enum_tuple(T{}, str), tuple_comp_rev) };
			result.first != result.second)
				return std::get<T>(*result.first);
		return {};
	}

	static constexpr auto enum_values { _values() };
	static constexpr auto enum_entries { _entries(std::make_index_sequence<enum_values.size()>()) };
	static constexpr auto enum_scoped_entries { _scoped_entries(std::make_index_sequence<enum_values.size()>()) };
	static constexpr auto enum_entries_sorted { _entries_sorted() };
	static constexpr auto enum_names { _names(std::make_index_sequence<enum_values.size()>()) };

	template<typename Fn, typename... Args>
	requires std::invocable<Fn&&, T, Args...>
	[[maybe_unused]] static constexpr auto for_each(Fn&& func, Args&&... args) noexcept
	{
		for (const auto ev : enum_values)
			std::invoke(std::forward<Fn>(func), ev, std::forward<Args>(args)...);
		return std::bind(std::forward<Fn>(func), std::placeholders::_1, std::forward<Args>(args)...);
	}
};

//-----------------------------------------------------------------------------------------
// bitset based on supplied enum
// Note: your enum sequence must be continuous with the last enum value < count of enumerations
//-----------------------------------------------------------------------------------------
template<typename T>
concept valid_bitset_enum = std::is_enum_v<T> && requires(T)
{
	requires conjure_enum<T>::count() > 0;
	requires static_cast<std::size_t>(conjure_enum<T>::enum_values.back()) < conjure_enum<T>::count();
};

template<valid_bitset_enum T>
class enum_bitset
{
	using U = std::underlying_type_t<T>;
	static constexpr auto countof { conjure_enum<T>::count() };

	template<T val>
	static constexpr U to_underlying() noexcept { return static_cast<U>(val); } // C++23: upgrade to std::to_underlying
	static constexpr U to_underlying(T val) noexcept { return static_cast<U>(val); }
	static constexpr U all_bits { (1 << countof) - 1 };
	U _present{};

public:
	constexpr enum_bitset(U bits) noexcept : _present(bits) {}
	constexpr enum_bitset(std::string_view from, bool anyscope=false, char sep='|', bool ignore_errors=true)
		: _present(factory(from, anyscope, sep, ignore_errors)) {}

	template<typename... E>
	requires(std::is_enum_v<E> && ...)
	constexpr enum_bitset(E... comp) noexcept : _present((0u | ... | (1 << to_underlying(comp)))) {}

	template<typename... I>
	requires(std::is_integral_v<I> && ...)
	constexpr enum_bitset(I... comp) noexcept : _present((0u | ... | (1 << comp))) {}

	constexpr enum_bitset() = default;
	constexpr ~enum_bitset() = default;

	constexpr std::size_t count() const noexcept
		{ return std::popcount(static_cast<std::make_unsigned_t<U>>(_present)); } // C++23: upgrade to std::bitset when count is constexpr
	constexpr std::size_t size() const noexcept { return countof; }
	constexpr U to_ulong() const noexcept { return _present; }
	constexpr unsigned long long to_ullong() const noexcept { return _present; }

	constexpr bool operator[](std::size_t pos) const noexcept { return test(pos); }
	constexpr bool operator[](T what) const noexcept { return test(what); }

	constexpr void set(U pos) noexcept { _present |= (1 << pos); }
	constexpr void set(T what) noexcept { set(to_underlying(what)); }
	constexpr void set() noexcept { _present = all_bits; }
	template<T what>
	constexpr void set() noexcept
	{
		if constexpr (constexpr auto uu{to_underlying<what>()}; uu < countof)
			_present |= (1 << uu);
	}
	template<T... comp>
	constexpr void set_all() noexcept { (set<comp>(),...); }
	template<typename... E>
	constexpr void set_all(E... comp) noexcept { return (... | (set(comp))); }

	template<T what>
	constexpr void flip() noexcept
	{
		if constexpr (constexpr auto uu{to_underlying<what>()}; uu < countof)
			_present ^= (1 << uu);
	}
	constexpr void flip() noexcept { _present = ~_present & all_bits; }
	constexpr void flip(U pos) noexcept { _present ^= (1 << pos); }
	constexpr void flip(T what) noexcept { flip(to_underlying(what)); }

	template<T what>
	constexpr void clear() noexcept
	{
		if constexpr (constexpr auto uu{to_underlying<what>()}; uu < countof)
			_present &= ~(1 << uu);
	}
	constexpr void clear() noexcept { _present = 0; }
	constexpr void clear(U pos) noexcept { _present &= ~(1 << pos); }
	constexpr void clear(T what) noexcept { clear(to_underlying(what)); }
	template<T... comp>
	constexpr void clear_all() noexcept { (clear<comp>(),...); }
	template<typename... I>
	requires(std::is_integral_v<I> && ...)
	constexpr void clear_all(I...comp) noexcept { (clear(comp),...); };

	constexpr bool test(U pos) const noexcept { return _present & (1 << pos); }
	constexpr bool test(T what) const noexcept { return test(to_underlying(what)); }
	constexpr bool test() const noexcept { return _present; }
	template<T what>
	constexpr bool test() const noexcept
	{
		if constexpr (constexpr auto uu{to_underlying<what>()}; uu < countof)
			return test(uu);
	}
	template<T... comp>
	constexpr bool test_any() const noexcept { return (... || test<comp>()); }
	template<typename... I>
	requires(std::is_integral_v<I> && ...)
	constexpr bool test_any(I...comp) const noexcept { return (... || test(comp)); }
	template<typename... E>
	requires(std::is_enum_v<E> && ...)
	constexpr bool test_any(E...comp) const noexcept { return (... || test(comp)); }
	template<T... comp>
	constexpr bool test_all() const noexcept { return (... && test<comp>()); }
	template<typename... I>
	requires(std::is_integral_v<I> && ...)
	constexpr bool test_all(I...comp) const noexcept { return (... && test(comp)); }
	template<typename... E>
	requires(std::is_enum_v<E> && ...)
	constexpr bool test_all(E...comp) const noexcept { return (... && test(comp)); }

	constexpr enum_bitset& operator<<=(std::size_t pos) noexcept { _present <<= pos; return *this; }
	constexpr enum_bitset& operator>>=(std::size_t pos) noexcept { _present >>= pos; return *this; }
	constexpr enum_bitset& operator&=(T other) noexcept { _present &= (1 << to_underlying(other)); return *this; }
	constexpr enum_bitset& operator|=(T other) noexcept { _present |= (1 << to_underlying(other)); return *this; }
	constexpr enum_bitset& operator^=(T other) noexcept { _present ^= (1 << to_underlying(other)); return *this; }
	constexpr enum_bitset& operator&=(U other) noexcept { _present &= other; return *this; }
	constexpr enum_bitset& operator|=(U other) noexcept { _present |= other; return *this; }
	constexpr enum_bitset& operator^=(U other) noexcept { _present ^= other; return *this; }

	constexpr enum_bitset operator<<(int pos) const noexcept { return enum_bitset(_present << pos); }
	constexpr enum_bitset operator>>(int pos) const noexcept { return enum_bitset(_present >> pos); }
	constexpr enum_bitset operator&(T other) const noexcept { return enum_bitset(_present & (1 << to_underlying(other))); }
	constexpr enum_bitset operator|(T other) const noexcept { return enum_bitset(_present | (1 << to_underlying(other))); }
	constexpr enum_bitset operator^(T other) const noexcept { return enum_bitset(_present ^ (1 << to_underlying(other))); }
	constexpr enum_bitset operator&(U other) const noexcept { return enum_bitset(_present & other); }
	constexpr enum_bitset operator|(U other) const noexcept { return enum_bitset(_present | other); }
	constexpr enum_bitset operator^(U other) const noexcept { return enum_bitset(_present ^ other); }
	constexpr enum_bitset operator~() const noexcept { return enum_bitset(~_present & all_bits); }

	constexpr operator bool() const noexcept { return count(); }

	template<typename Fn, typename... Args>
	requires std::invocable<Fn&&, T, Args...>
	[[maybe_unused]] constexpr auto for_each(Fn&& func, Args&&... args) noexcept
	{
		for (const auto ev : conjure_enum<T>::enum_values)
			if (test(ev))
				std::invoke(std::forward<Fn>(func), ev, std::forward<Args>(args)...);
		return std::bind(std::forward<Fn>(func), std::placeholders::_1, std::forward<Args>(args)...);
	}

	/// create a bitset from enum separated enum string
	constexpr static U factory(std::string_view src, bool anyscope, char sep, bool ignore_errors)
	{
		enum_bitset result;
		auto trim([](std::string_view src) noexcept ->std::string_view
		{
		   const auto bg(src.find_first_not_of(" \t"));
			return bg == std::string_view::npos ? src : src.substr(bg, src.find_last_not_of(" \t") - bg + 1);
		});
		auto process([anyscope,&result](std::string_view src) noexcept ->bool
		{
			if (anyscope && !conjure_enum<T>::has_scope(src))
				src = conjure_enum<T>::add_scope(src);
			if (auto ev { conjure_enum<T>::string_to_enum(src) }; ev)
			{
				result |= *ev;
				return true;
			}
			return false;
		});
		for (std::string_view::size_type pos{}, fnd{};; pos = fnd + 1)
		{
			if ((fnd = src.find_first_of(sep, pos)) != std::string_view::npos)
			{
				if (auto srcp { trim(src.substr(pos, fnd - pos)) }; !process(trim(srcp)) && !ignore_errors)
					throw std::invalid_argument(std::string(srcp).c_str());
				continue;
			}
			if (pos < src.size())
				if (auto srcp { trim(src.substr(pos, src.size() - pos)) }; !process(trim(srcp)) && !ignore_errors)
					throw std::invalid_argument(std::string(srcp).c_str());
			break;
		}
      return result._present;
	}

	constexpr std::string to_string(char zero='0', char one='1') const noexcept
	{
		return std::bitset<countof>(_present).to_string(zero, one);
	}

	friend constexpr std::ostream& operator<<(std::ostream& os, const enum_bitset& what) noexcept
	{
		return os << what.to_string();
	}
};

template<typename T>
constexpr enum_bitset<T> operator&(const enum_bitset<T>& lh, const enum_bitset<T>& rh) noexcept
	{ return lh.operator&(rh.to_ulong()); }
template<typename T>
constexpr enum_bitset<T> operator|(const enum_bitset<T>& lh, const enum_bitset<T>& rh) noexcept
	{ return lh.operator|(rh.to_ulong()); }
template<typename T>
constexpr enum_bitset<T> operator^(const enum_bitset<T>& lh, const enum_bitset<T>& rh) noexcept
	{ return lh.operator^(rh.to_ulong()); }

//-----------------------------------------------------------------------------------------
} // FIX8

#endif // FIX8_CONJURE_ENUM_HPP_

