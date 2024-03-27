//-----------------------------------------------------------------------------------------
// conjure_enum (header only)
// Copyright (C) 2024 Fix8 Market Technologies Pty Ltd
//   by David L. Dight
// see https://github.com/fix8mt/conjure_enum
//
//		Based on magic_enum <https://github.com/Neargye/magic_enum>
//		Licensed under the MIT License <http://opensource.org/licenses/MIT>.
//		SPDX-License-Identifier: MIT
//		Copyright (c) 2019 - 2024 Daniil Goncharov <neargye@gmail.com>.
//
// Lightweight header-only C++20 enum reflection
//
// Distributed under the Boost Software License, Version 1.0 August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
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
#include <bitset>
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
	std::array<char, N + 1> _buff{};
public:
	constexpr fixed_string(std::string_view sv) noexcept { std::copy(sv.cbegin(), sv.cend(), _buff.begin()); }
	constexpr fixed_string() = delete;
	constexpr std::string_view get() const noexcept { return { _buff.data(), N }; }
	constexpr operator std::string_view() const noexcept { return get(); }
	constexpr std::size_t size() const noexcept { return _buff.size(); }
	friend std::ostream& operator<<(std::ostream& os, const fixed_string& what) { return os << what.get(); }
};

class conjure_enum
{
	static constexpr int enum_min_value{ENUM_MIN_VALUE}, enum_max_value{ENUM_MAX_VALUE};

	template<typename T>
	using enum_tuple = std::tuple<T, std::string_view>;

	template<typename T>
	using scoped_tuple = std::tuple<std::string_view, std::string_view>;

	template<typename T, T e>
	requires std::is_enum_v<T>
	static consteval const char *epeek() noexcept { return std::source_location::current().function_name(); }

	template<typename T>
	requires std::is_enum_v<T>
	static consteval const char *tpeek() noexcept { return std::source_location::current().function_name(); }

	template<typename T, std::size_t... I>
	static constexpr auto entries(std::index_sequence<I...>) noexcept
	{
		return std::array<enum_tuple<T>, sizeof...(I)>{{{ enum_values<T>[I], enum_name_v<T, enum_values<T>[I]>}...}};
	}

	template<typename T, std::size_t... I>
	static constexpr auto scoped_entries(std::index_sequence<I...>) noexcept
	{
		std::array<scoped_tuple<T>, sizeof...(I)> tmp{{{ remove_scope<T>(enum_name_v<T, enum_values<T>[I]>), enum_name_v<T, enum_values<T>[I]>}...}};
		std::sort(tmp.begin(), tmp.end(), scoped_comp<T>);
		return tmp;
	}

	template<typename T>
	static constexpr auto entries_sorted() noexcept
	{
		auto tmp { enum_entries<T> };
		std::sort(tmp.begin(), tmp.end(), tuple_comp_rev<T>);
		return tmp;
	}

	template<typename T, std::size_t... I>
	static constexpr auto names(std::index_sequence<I...>) noexcept
	{
		return std::array<std::string_view, sizeof...(I)>{{{ enum_name_v<T, enum_values<T>[I]>}...}};
	}

	template<typename T, std::size_t... I>
	static constexpr auto values(std::index_sequence<I...>) noexcept
	{
		constexpr std::array<bool, sizeof...(I)> valid { is_valid<T, static_cast<T>(enum_min_value + I)>()... };
		constexpr auto num_valid { std::count_if(valid.cbegin(), valid.cend(), [](bool val) noexcept { return val; }) };
		static_assert(num_valid > 0, "empty enums not supported");
		std::array<T, num_valid> vals{};
		for(std::size_t offset{}, nn{}; nn < num_valid; ++offset)
			if (valid[offset])
				vals[nn++] = static_cast<T>(enum_min_value + offset);
		return vals;
	}

	template<typename T>
	static constexpr bool value_comp(const T& pl, const T& pr) noexcept
	{
		return static_cast<int>(pl) < static_cast<int>(pr);
	}
	template<typename T>
	static constexpr bool tuple_comp(const enum_tuple<T>& pl, const enum_tuple<T>& pr) noexcept
	{
		return static_cast<int>(std::get<T>(pl)) < static_cast<int>(std::get<T>(pr));
	}
	template<typename T>
	static constexpr bool tuple_comp_rev(const enum_tuple<T>& pl, const enum_tuple<T>& pr) noexcept
	{
		return std::get<std::string_view>(pl) < std::get<std::string_view>(pr);
	}
	template<typename T>
	static constexpr bool scoped_comp(const scoped_tuple<T>& pl, const scoped_tuple<T>& pr) noexcept
	{
		return std::get<0>(pl) < std::get<0>(pr);
	}

public:
	conjure_enum() = delete;

	template<typename T, T e>
	requires std::is_enum_v<T>
	static constexpr std::string_view get_name() noexcept
	{
		/*
			static consteval const char* epeek() [with T = unscoped name; T e = <value>]
			static consteval const char* epeek() [with T = scoped name; T e = <scoped::value>]
		*/
		constexpr std::string_view from{epeek<T, e>()};
#if defined(__clang__) || defined(__GNUC__)
		if (constexpr auto ep { from.rfind("e = ") }; ep != std::string_view::npos && from[ep + 4] != '(')
		{
			std::string_view result { from.substr(ep + 4) };
			if (auto lc { result.find_first_of(']') }; lc != std::string_view::npos)
				return result.substr(0, lc);
		}
#elif defined(_MSC_VER)
# error "incomplete"
#else
# error "compiler not supported"
#endif
		return {};
	}

	template<typename T>
	requires std::is_enum_v<T>
	static constexpr std::string_view get_type() noexcept
	{
		/*
			static consteval const char* conjure_enum::tpeek() [with T = <type>]
		*/
		constexpr std::string_view from{tpeek<T>()};
#if defined(__clang__) || defined(__GNUC__)
		if (constexpr auto ep { from.rfind("T = ") }; ep != std::string_view::npos)
		{
			std::string_view result { from.substr(ep + 4) };
			if (auto lc { result.find_first_of(']') }; lc != std::string_view::npos)
				return result.substr(0, lc);
		}
#elif defined(_MSC_VER)
# error "incomplete"
#else
# error "compiler not supported"
#endif
		return {};
	}

	template<typename T>
	struct is_scoped : std::integral_constant<bool, requires
	{
		requires std::is_enum_v<T>;
		requires !std::is_convertible_v<T, std::underlying_type_t<T>>;
	}>{};

	template<typename T, T e>
	static constexpr bool is_valid() noexcept { return !get_name<T, e>().empty(); }

	template<typename T>
	static constexpr auto values() noexcept
	{
		return values<T>(std::make_index_sequence<enum_max_value - enum_min_value + 1>({}));
	}

	template<typename T>
	static constexpr auto count() noexcept { return enum_values<T>.size(); }

	template<typename T, T e>
	static constexpr auto enum_name() noexcept
	{
		constexpr auto result { get_name<T, e>() };
		return fixed_string<result.size()>(result);
	}

	template<typename T>
	static constexpr std::string_view remove_scope(std::string_view what) noexcept
	{
		if constexpr (is_scoped<T>())
		{
			if (auto lc { what.find_last_of(':') }; lc != std::string_view::npos)
				return what.substr(lc + 1);
		}
		return what;
	}

	template<typename T>
	static constexpr std::string_view add_scope(std::string_view what) noexcept
	{
		if constexpr (is_scoped<T>())
		{
			if (const auto result { std::equal_range(enum_scoped_entries<T>.cbegin(),
				enum_scoped_entries<T>.cend(), scoped_tuple<T>(what, std::string_view()), scoped_comp<T>) };
					result.first != result.second)
						return std::get<1>(*result.first);
		}
		return what;
	}

	template<typename T>
	static constexpr auto cbegin() noexcept { return enum_entries<T>.cbegin(); }

	template<typename T>
	static constexpr auto cend() noexcept { return enum_entries<T>.cend(); }

	template<typename T>
	static constexpr std::optional<T> int_to_enum(int value) noexcept
	{
		if (const auto result { std::equal_range(enum_values<T>.cbegin(), enum_values<T>.cend(), static_cast<T>(value), value_comp<T>) };
			result.first != result.second)
				return *result.first;
		return {};
	}

	template<typename T>
	static constexpr bool enum_contains(T value) noexcept
	{
		const auto result { std::equal_range(enum_values<T>.cbegin(), enum_values<T>.cend(), value, value_comp<T>) };
		return result.first != result.second;
   }

	template<typename T>
	static constexpr bool enum_contains(std::string_view str) noexcept
	{
		const auto result { std::equal_range(enum_entries_sorted<T>.cbegin(), enum_entries_sorted<T>.cend(), enum_tuple<T>(T{}, str), tuple_comp_rev<T>) };
		return result.first != result.second;
	}

	template<typename T>
	static constexpr std::string_view enum_to_string(T value) noexcept
	{
		if (const auto result { std::equal_range(enum_entries<T>.cbegin(), enum_entries<T>.cend(), enum_tuple<T>(value, std::string_view()), tuple_comp<T>) };
			result.first != result.second)
				return std::get<std::string_view>(*result.first);
		return {};
	}

	template<typename T>
	static constexpr std::optional<T> string_to_enum(std::string_view str) noexcept
	{
		if (const auto result { std::equal_range(enum_entries_sorted<T>.cbegin(), enum_entries_sorted<T>.cend(), enum_tuple<T>(T{}, str), tuple_comp_rev<T>) };
			result.first != result.second)
				return std::get<T>(*result.first);
		return {};
	}

	template<typename T>
	static constexpr auto enum_values { values<T>() };

	template<typename T>
	static constexpr auto enum_entries { entries<T>(std::make_index_sequence<enum_values<T>.size()>()) };

	template<typename T>
	static constexpr auto enum_scoped_entries { scoped_entries<T>(std::make_index_sequence<enum_values<T>.size()>()) };

	template<typename T>
	static constexpr auto enum_entries_sorted { entries_sorted<T>() };

	template<typename T>
	static constexpr auto enum_names { names<T>(std::make_index_sequence<enum_values<T>.size()>()) };

	template<typename T, T e>
	static constexpr auto enum_name_v { enum_name<T, e>() };

	template<typename T, typename Fn, typename... Args>
	requires std::invocable<Fn&&, T, Args...>
	[[maybe_unused]] static constexpr auto for_each(Fn&& func, Args&&... args) noexcept
	{
		for (const auto ev : enum_values<T>)
			std::invoke(std::forward<Fn>(func), ev, std::forward<Args>(args)...);
		return std::bind(std::forward<Fn>(func), std::placeholders::_1, std::forward<Args>(args)...);
	}
};

//-----------------------------------------------------------------------------------------
// bitset based on supplied enum
// Note: the enum sequence must be contiguous with the last enum value < count of enumerations
//-----------------------------------------------------------------------------------------
template<typename T, std::size_t countof=conjure_enum::count<T>()>
requires (std::is_enum_v<T> && countof > 0 && static_cast<std::size_t>(conjure_enum::enum_values<T>.back()) < countof)
class enum_bitset
{
	using U = std::underlying_type_t<T>;

	static constexpr bool scoped { !std::is_same_v<U, T> };
	template<T val>
	static constexpr U as_U() noexcept { return static_cast<U>(val); }

	static constexpr U as_U(T val) noexcept { return static_cast<U>(val); }
	static constexpr U all_bits {(1 << countof) - 1};
	U _present{};

public:
	constexpr enum_bitset(U bits) noexcept : _present(bits) {}
	template<typename... E>
	constexpr enum_bitset(E... comp) noexcept : _present((... | (1 << as_U(comp)))) {}
	constexpr enum_bitset() = default;
	constexpr ~enum_bitset() = default;

	constexpr std::size_t count() const noexcept { return std::popcount(static_cast<std::make_unsigned_t<U>>(_present)); } // upgrade to std::bitset when constexpr in c++23
	constexpr std::size_t size() const noexcept { return countof; }
	constexpr U to_ulong() const noexcept { return _present; }

	constexpr bool operator[](std::size_t pos) const noexcept { return test(pos); }
	constexpr bool operator[](T what) const noexcept { return test(what); }

	template<typename... E>
	static constexpr U bitsum(E... comp) noexcept { return (... | (1 << comp)); }

	template<T... comp>
	static constexpr U bitsum() noexcept { return (... | (1 << comp)); }

	constexpr void set(std::size_t pos) noexcept { _present |= (1 << pos); }
	constexpr void set(T what) noexcept { set(as_U(what)); }
	constexpr void set() noexcept { _present = all_bits; }
	template<T what>
	constexpr void set() noexcept
	{
		if constexpr (constexpr auto uu{as_U<what>()}; uu < countof)
			_present |= (1 << uu);
	}
	template<T... comp>
	constexpr void set_all() noexcept { (set<comp>(),...); }
	template<typename... E>
	constexpr void set(E... comp) noexcept { return (... | (set(comp))); }

	template<T what>
	constexpr void flip() noexcept
	{
		if constexpr (constexpr auto uu{as_U<what>()}; uu < countof)
			_present ^= (1 << uu);
	}
	constexpr void flip() noexcept { _present = ~_present & all_bits; }
	constexpr void flip(std::size_t pos) noexcept { _present ^= (1 << pos); }
	constexpr void flip(T what) noexcept { flip(as_U(what)); }

	template<T what>
	constexpr void clear() noexcept
	{
		if constexpr (constexpr auto uu{as_U<what>()}; uu < countof)
			_present &= ~(1 << uu);
	}
	constexpr void clear() noexcept { _present = 0; }
	constexpr void clear(std::size_t pos) noexcept { _present &= ~(1 << pos); }
	constexpr void clear(T what) noexcept { clear(as_U(what)); }
	template<T... comp>
	constexpr void clear_all() noexcept { (clear<comp>(),...); }

	constexpr bool test(std::size_t pos) const noexcept { return _present & (1 << pos); }
	constexpr bool test(T what) const noexcept { return test(as_U(what)); }
	constexpr bool test() const noexcept { return _present; }
	template<T what>
	constexpr bool test() const noexcept
	{
		if constexpr (constexpr auto uu{as_U<what>()}; uu < countof)
			return test(uu);
	}
	template<T... comp>
	constexpr U test_any() const noexcept { return (... || test<comp>()); }
	template<T... comp>
	constexpr U test_all() const noexcept { return (... && test<comp>()); }

	constexpr U& operator<<=(std::size_t pos) noexcept { return _present <<= pos; }
	constexpr U& operator>>=(std::size_t pos) noexcept { return _present >>= pos; }
	constexpr U& operator&=(T other) noexcept { return _present &= (1 << as_U(other)); }
	constexpr U& operator|=(T other) noexcept { return _present |= (1 << as_U(other)); }
	constexpr U& operator^=(T other) noexcept { return _present ^= (1 << as_U(other)); }
	constexpr U& operator&=(U other) noexcept { return _present &= other; }
	constexpr U& operator|=(U other) noexcept { return _present |= other; }
	constexpr U& operator^=(U other) noexcept { return _present ^= other; }

	constexpr U operator<<(int pos) const noexcept { return _present << pos; }
	constexpr U operator>>(int pos) const noexcept { return _present >> pos; }
	constexpr U operator&(T other) const noexcept { return _present & (1 << as_U(other)); }
	constexpr U operator|(T other) const noexcept { return _present | (1 << as_U(other)); }
	constexpr U operator^(T other) const noexcept { return _present ^ (1 << as_U(other)); }
	constexpr U operator&(U other) const noexcept { return _present & other; }
	constexpr U operator|(U other) const noexcept { return _present | other; }
	constexpr U operator^(U other) const noexcept { return _present ^ other; }
	constexpr U operator~() const noexcept { return ~_present & all_bits; }

	constexpr operator bool() const noexcept { return count(); }

	template<typename Fn, typename... Args>
	requires std::invocable<Fn&&, T, Args...>
	[[maybe_unused]] constexpr auto for_each(Fn&& func, Args&&... args) noexcept
	{
		for (const auto ev : conjure_enum::enum_values<T>)
			if (test(ev))
				std::invoke(std::forward<Fn>(func), ev, std::forward<Args>(args)...);
		return std::bind(std::forward<Fn>(func), std::placeholders::_1, std::forward<Args>(args)...);
	}

	constexpr std::string to_string(char zero='0', char one='1') const noexcept
	{
		return std::bitset<countof>(_present).to_string(zero, one);
	}

	friend std::ostream& operator<<(std::ostream& os, const enum_bitset& what) noexcept
	{
		return os << what.to_string();
	}
};

//-----------------------------------------------------------------------------------------
} // FIX8

#endif // FIX8_CONJURE_ENUM_HPP_

