//-----------------------------------------------------------------------------------------
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2024 Fix8 Market Technologies Pty Ltd
// SPDX-FileType: SOURCE
//
// conjure_enum (header only)
//   by David L. Dight
// see https://github.com/fix8mt/conjure_enum
//
// Lightweight header-only C++20 enum and type reflection
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
#ifndef FIX8_CONJURE_ENUM_HPP_
#define FIX8_CONJURE_ENUM_HPP_

#if defined _MSC_VER && (_MSC_VER < 1910) || !defined _MSC_VER && (__cplusplus < 202002L)
# error "C++20 not supported by your compiler"
#endif

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
	explicit constexpr fixed_string(std::string_view sv) noexcept : fixed_string{sv.data(), std::make_integer_sequence<char, N>{}} {}
	constexpr fixed_string() = delete;
	constexpr std::string_view get() const noexcept { return { _buff.data(), N }; }
	constexpr operator std::string_view() const noexcept { return get(); }
	constexpr char operator[](size_t idx) const noexcept { return _buff[idx]; }
	constexpr std::size_t size() const noexcept { return _buff.size(); }
	friend std::ostream& operator<<(std::ostream& os, const fixed_string& what) noexcept { return os << what.get(); }
};

//-----------------------------------------------------------------------------------------
// compiler specifics
//-----------------------------------------------------------------------------------------
class cs
{
	static constexpr auto _specifics
	{
		std::to_array<std::tuple<std::string_view, char, std::string_view, char>>
		({
#if defined __clang__
			{ "e = ", ']', "(anonymous namespace)", '(' }, { "T = ", ']', "(anonymous namespace)", '(' },
#elif defined __GNUC__
			{ "e = ", ';', "<unnamed>", '<' }, { "T = ", ']', "{anonymous}", '{' },
#elif defined _MSC_VER
			{ "epeek<", '>', "`anonymous-namespace'", '`' }, { "enum ", '>', "enum `anonymous-namespace'", '`' },
#else
# error "conjure_enum not supported by your compiler"
#endif
		})
	};

public:
	cs() = delete;
	~cs() = delete;
	cs(const cs&) = delete;
	cs& operator=(const cs&) = delete;
	cs(cs&&) = delete;
	cs& operator=(cs&&) = delete;

	template<int N, int V> // can't have constexpr decompositions! (but why not?)
	static constexpr auto get_spec() noexcept
	{
		return std::get<N>(_specifics[V]);
	}
};

//-----------------------------------------------------------------------------------------
template<typename T>
concept valid_enum = requires(T)
{
	requires std::same_as<T, std::decay_t<T>>;
	requires std::is_enum_v<T>;
};

//-----------------------------------------------------------------------------------------
template<valid_enum T>
class conjure_enum
{
	static constexpr int enum_min_value{ENUM_MIN_VALUE}, enum_max_value{ENUM_MAX_VALUE};
	static_assert(enum_max_value > enum_min_value, "ENUM_MAX_VALUE must be greater than ENUM_MIN_VALUE");

public:
	conjure_enum() = delete;
	~conjure_enum() = delete;
	conjure_enum(const conjure_enum&) = delete;
	conjure_enum& operator=(const conjure_enum&) = delete;
	conjure_enum(conjure_enum&&) = delete;
	conjure_enum& operator=(conjure_enum&&) = delete;

	using enum_tuple = std::tuple<T, std::string_view>;
	using scoped_tuple = std::tuple<std::string_view, std::string_view>;

private:
	template<T e>
	static constexpr auto _enum_name() noexcept
	{
		constexpr auto result { _get_name<e>() };
		return fixed_string<result.size()>(result);
	}

	template<T e>
	static constexpr auto _enum_name_v { _enum_name<e>() };

	template<std::size_t... I>
	static constexpr auto _entries(std::index_sequence<I...>) noexcept
	{
		return std::array<enum_tuple, sizeof...(I)>{{{ values[I], _enum_name_v<values[I]>}...}};
	}

	template<std::size_t... I>
	static constexpr auto _unscoped_entries(std::index_sequence<I...>) noexcept
	{
		std::array<enum_tuple, sizeof...(I)> tmp{{{ values[I], _remove_scope(_enum_name_v<values[I]>)}...}};
		std::sort(tmp.begin(), tmp.end(), tuple_comp_rev);
		return tmp;
	}

	static constexpr std::string_view _remove_scope(std::string_view what) noexcept
	{
		if (const auto lc { what.find_last_of(':') }; lc != std::string_view::npos)
			return what.substr(lc + 1);
		return what;
	}

	template<std::size_t... I>
	static constexpr auto _scoped_entries(std::index_sequence<I...>) noexcept
	{
		std::array<scoped_tuple, sizeof...(I)> tmp{{{ _remove_scope(_enum_name_v<values[I]>), _enum_name_v<values[I]>}...}};
		std::sort(tmp.begin(), tmp.end(), scoped_comp);
		return tmp;
	}

	template<std::size_t... I>
	static constexpr auto _rev_scoped_entries(std::index_sequence<I...>) noexcept
	{
		std::array<scoped_tuple, sizeof...(I)> tmp{{{ _enum_name_v<values[I]>, _remove_scope(_enum_name_v<values[I]>)}...}};
		std::sort(tmp.begin(), tmp.end(), scoped_comp);
		return tmp;
	}

	static constexpr auto _sorted_entries() noexcept
	{
		auto tmp { entries };
		std::sort(tmp.begin(), tmp.end(), tuple_comp_rev);
		return tmp;
	}

	template<std::size_t... I>
	static constexpr auto _names(std::index_sequence<I...>) noexcept
	{
		return std::array<std::string_view, sizeof...(I)>{{{ _enum_name_v<values[I]>}...}};
	}

	template<std::size_t... I>
	static constexpr auto _unscoped_names(std::index_sequence<I...>) noexcept
	{
		return std::array<std::string_view, sizeof...(I)>{{{ _remove_scope(_enum_name_v<values[I]>)}...}};
	}

	template<T e>
	static constexpr bool _is_valid() noexcept
	{
		constexpr std::string_view from{epeek<e>()};
		constexpr auto ep { from.rfind(cs::get_spec<0,0>()) };
		if constexpr (ep == std::string_view::npos)
			return false;
#if defined __clang__
		if constexpr (from[ep + cs::get_spec<0,0>().size()] == '(')
		{
			if constexpr (from[ep + cs::get_spec<0,0>().size() + 1] == '(')
				return false; // check for ((anonymous namespace)
			if constexpr (constexpr auto lstr { from.substr(ep + cs::get_spec<0,0>().size()) }; lstr.find(cs::get_spec<2,0>()) != std::string_view::npos)	// is anon
				return true;
		}
		else if constexpr (from.substr(ep + cs::get_spec<0,0>().size()).find_first_of(cs::get_spec<1,0>()) != std::string_view::npos)
			return true;
		return false;
#else
		if constexpr (from[ep + cs::get_spec<0,0>().size()] != '('
			&& from.substr(ep + cs::get_spec<0,0>().size()).find_first_of(cs::get_spec<1,0>()) != std::string_view::npos)
				return true;
		else
			return false;
#endif
	}

#if defined BUILD_CONSRVCPP20
	template<std::size_t N>
	static constexpr auto count_if_constexpr(const bool (&valid)[N]) noexcept
	{
		std::size_t cnt{};
		for(std::size_t nn{}; nn < N; ++nn)
			if (valid[nn])
				++cnt;
		return cnt;
	}
#endif

	template<std::size_t... I>
	static constexpr auto _values(std::index_sequence<I...>) noexcept
	{
#if defined BUILD_CONSRVCPP20
		constexpr bool valid[sizeof...(I)] { _is_valid<static_cast<T>(enum_min_value + I)>()... };
		constexpr auto num_valid { count_if_constexpr(valid) };
#else
		constexpr std::array<bool, sizeof...(I)> valid { _is_valid<static_cast<T>(enum_min_value + I)>()... };
		constexpr auto num_valid { std::count_if(valid.cbegin(), valid.cend(), [](bool val) noexcept { return val; }) };
#endif
		static_assert(num_valid > 0, "conjure_enum requires non-empty enum");
		std::array<T, num_valid> vals{};
		for(std::size_t offset{}, nn{}; nn < num_valid; ++offset)
			if (valid[offset])
				vals[nn++] = static_cast<T>(enum_min_value + offset);
		return vals;
	}

	static constexpr auto _values() noexcept
	{
		return _values(std::make_index_sequence<enum_max_value - enum_min_value + 1>({}));
	}

	template<T e>
	static constexpr std::string_view _get_name() noexcept
	{
		constexpr std::string_view from{epeek<e>()};
		constexpr auto ep { from.rfind(cs::get_spec<0,0>()) };
		if constexpr (ep == std::string_view::npos)
			return {};
		if constexpr (from[ep + cs::get_spec<0,0>().size()] == cs::get_spec<3,0>())
		{
#if defined __clang__
			if constexpr (from[ep + cs::get_spec<0,0>().size() + 1] == cs::get_spec<3,0>())
				return {}; // check for ((anonymous namespace)
#endif
			if (constexpr auto lstr { from.substr(ep + cs::get_spec<0,0>().size()) }; lstr.find(cs::get_spec<2,0>()) != std::string_view::npos)	// is anon
				if constexpr (constexpr auto lc { lstr.find_first_of(cs::get_spec<1,0>()) }; lc != std::string_view::npos)
					return lstr.substr(cs::get_spec<2,0>().size() + 2, lc - (cs::get_spec<2,0>().size() + 2)); // eat "::"
		}
		constexpr std::string_view result { from.substr(ep + cs::get_spec<0,0>().size()) };
		if constexpr (constexpr auto lc { result.find_first_of(cs::get_spec<1,0>()) }; lc != std::string_view::npos)
			return result.substr(0, lc);
		else
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
	static consteval const char *tpeek() noexcept { return std::source_location::current().function_name(); }

	template<T e>
	static consteval const char *epeek() noexcept { return std::source_location::current().function_name(); }

	template<T e>
	static constexpr std::string_view enum_to_string() noexcept { return _get_name<e>(); }

	static constexpr std::string_view type_name() noexcept
	{
		constexpr std::string_view from{tpeek()};
		if constexpr (constexpr auto ep { from.rfind(cs::get_spec<0,1>()) }; ep != std::string_view::npos)
		{
			constexpr std::string_view result { from.substr(ep + cs::get_spec<0,1>().size()) };
			if constexpr (constexpr auto lc { result.find_first_of(cs::get_spec<1,1>()) }; lc != std::string_view::npos)
				return result.substr(0, lc);
		}
		else
			return {};
	}

	struct is_scoped : std::integral_constant<bool, requires
	{
		requires !std::convertible_to<T, std::underlying_type_t<T>>;
	}>{};

	template<T e>
	static constexpr bool is_valid() noexcept { return _is_valid<e>(); }

	static constexpr auto count() noexcept { return values.size(); }

	static constexpr std::string_view remove_scope(std::string_view what) noexcept
	{
		if constexpr (is_scoped())
		{
			if (const auto result { std::equal_range(rev_scoped_entries.cbegin(),
				rev_scoped_entries.cend(), scoped_tuple(what, std::string_view()), scoped_comp) };
					result.first != result.second)
						return std::get<1>(*result.first);
		}
		return what;
	}

	static constexpr std::string_view add_scope(std::string_view what) noexcept
	{
		if constexpr (is_scoped())
		{
			if (const auto result { std::equal_range(scoped_entries.cbegin(),
				scoped_entries.cend(), scoped_tuple(what, std::string_view()), scoped_comp) };
					result.first != result.second)
						return std::get<1>(*result.first);
		}
		return what;
	}

	static constexpr bool has_scope(std::string_view what) noexcept
	{
		if constexpr (is_scoped())
			return contains(what);
		else
			return false;
	}

	static constexpr auto cbegin() noexcept { return entries.cbegin(); }
	static constexpr auto cend() noexcept { return entries.cend(); }
	static constexpr auto crbegin() noexcept { return entries.crbegin(); }
	static constexpr auto crend() noexcept { return entries.crend(); }
	static constexpr auto front() noexcept { return *cbegin(); }
	static constexpr auto back() noexcept { return *std::prev(cend()); }

	static constexpr int enum_to_int(T value) noexcept
	{
		return static_cast<int>(value);
	}
	static constexpr std::underlying_type_t<T> enum_to_underlying(T value) noexcept
	{
		return static_cast<std::underlying_type_t<T>>(value);
	}
	static constexpr std::optional<T> int_to_enum(int value) noexcept
	{
		if (const auto result { std::equal_range(values.cbegin(), values.cend(), static_cast<T>(value), value_comp) };
			result.first != result.second)
				return *result.first;
		return {};
	}
	static constexpr bool contains(T value) noexcept
	{
		const auto result { std::equal_range(values.cbegin(), values.cend(), value, value_comp) };
		return result.first != result.second;
   }
	static constexpr bool contains(std::string_view str) noexcept
	{
		const auto result { std::equal_range(sorted_entries.cbegin(), sorted_entries.cend(), enum_tuple(T{}, str), tuple_comp_rev) };
		return result.first != result.second;
	}
	static constexpr std::string_view enum_to_string(T value, bool noscope=false) noexcept
	{
		if (const auto result { std::equal_range(entries.cbegin(), entries.cend(), enum_tuple(value, std::string_view()), tuple_comp) };
			result.first != result.second)
				return noscope ? remove_scope(std::get<std::string_view>(*result.first)) : std::get<std::string_view>(*result.first);
		return {};
	}
	static constexpr std::optional<T> string_to_enum(std::string_view str) noexcept
	{
		if (const auto result { std::equal_range(sorted_entries.cbegin(), sorted_entries.cend(), enum_tuple(T{}, str), tuple_comp_rev) };
			result.first != result.second)
				return std::get<T>(*result.first);
		return {};
	}
	static constexpr std::optional<T> unscoped_string_to_enum(std::string_view str) noexcept
	{
		if (const auto result { std::equal_range(unscoped_entries.cbegin(), unscoped_entries.cend(), enum_tuple(T{}, str), tuple_comp_rev) };
			result.first != result.second)
				return std::get<T>(*result.first);
		return {};
	}

	template<typename Fn, typename... Args>
	requires std::invocable<Fn&&, T, Args...>
	[[maybe_unused]] static constexpr auto for_each(Fn&& func, Args&&... args) noexcept
	{
		for (const auto ev : values)
			std::invoke(std::forward<Fn>(func), ev, std::forward<Args>(args)...);
		return std::bind(std::forward<Fn>(func), std::placeholders::_1, std::forward<Args>(args)...);
	}

	// specialisation for member function with object
	template<typename Fn, typename C, typename... Args>
	requires std::invocable<Fn&&, C, T, Args...>
	[[maybe_unused]] static constexpr auto for_each(Fn&& func, C *obj, Args&&... args) noexcept
	{
		return for_each(std::bind(std::forward<Fn>(func), obj, std::placeholders::_1, std::forward<Args>(args)...));
	}

	// public constexpr data structures
	static constexpr auto values { _values() };
	static constexpr auto entries { _entries(std::make_index_sequence<values.size()>()) };
	static constexpr auto scoped_entries { _scoped_entries(std::make_index_sequence<values.size()>()) };
	static constexpr auto unscoped_entries { _unscoped_entries(std::make_index_sequence<values.size()>()) };
	static constexpr auto rev_scoped_entries { _rev_scoped_entries(std::make_index_sequence<values.size()>()) };
	static constexpr auto sorted_entries { _sorted_entries() };
	static constexpr auto names { _names(std::make_index_sequence<values.size()>()) };
	static constexpr auto unscoped_names { _unscoped_names(std::make_index_sequence<values.size()>()) };
};

//-----------------------------------------------------------------------------------------
// allow range based for
template<valid_enum T>
struct iterator_adaptor
{
	constexpr auto begin() noexcept { return conjure_enum<T>::entries.cbegin(); }
	constexpr auto end() noexcept { return conjure_enum<T>::entries.cend(); }
};

//-----------------------------------------------------------------------------------------
// ostream& operator<< for any enum; add the following before using:
// using ostream_enum_operator::operator<<;
//-----------------------------------------------------------------------------------------
namespace ostream_enum_operator
{
	template<typename CharT, typename Traits=std::char_traits<CharT>, valid_enum T>
	constexpr std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, T value) noexcept
	{
		if (conjure_enum<T>::contains(value))
			return os << conjure_enum<T>::enum_to_string(value);
		return os << conjure_enum<T>::enum_to_underlying(value);
	}
}

//-----------------------------------------------------------------------------------------
// bitset based on supplied enum
// Note: your enum sequence must be continuous with the last enum value < count of enumerations
//-----------------------------------------------------------------------------------------
template<valid_enum T>
requires (conjure_enum<T>::count() > 0) && (static_cast<std::size_t>(conjure_enum<T>::values.back()) < conjure_enum<T>::count())
class enum_bitset
{
	using U = std::underlying_type_t<std::decay_t<T>>;
	static constexpr auto countof { conjure_enum<T>::count() };

	template<T val>
	static constexpr U to_underlying() noexcept { return static_cast<U>(val); } // C++23: upgrade to std::to_underlying
	static constexpr U to_underlying(T val) noexcept { return static_cast<U>(val); }
	static constexpr U all_bits { (1 << countof) - 1 };
	U _present{};

public:
	explicit constexpr enum_bitset(U bits) noexcept : _present(bits) {}
	constexpr enum_bitset(std::string_view from, bool anyscope=false, char sep='|', bool ignore_errors=true)
		: _present(factory(from, anyscope, sep, ignore_errors)) {}

	template<valid_enum... E>
	constexpr enum_bitset(E... comp) noexcept : _present((0u | ... | (1 << to_underlying(comp)))) {}

	template<std::integral... I>
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

	template<valid_enum... E>
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
	constexpr void reset() noexcept
	{
		if constexpr (constexpr auto uu{to_underlying<what>()}; uu < countof)
			_present &= ~(1 << uu);
	}

	constexpr void reset() noexcept { _present = 0; }
	constexpr void reset(U pos) noexcept { _present &= ~(1 << pos); }
	constexpr void reset(T what) noexcept { reset(to_underlying(what)); }

	template<T... comp>
	constexpr void reset_all() noexcept { (reset<comp>(),...); }

	template<std::integral... I>
	constexpr void reset_all(I...comp) noexcept { (reset(comp),...); }

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

	template<std::integral... I>
	constexpr bool test_any(I...comp) const noexcept { return (... || test(comp)); }

	template<valid_enum... E>
	constexpr bool test_any(E...comp) const noexcept { return (... || test(comp)); }

	template<T... comp>
	constexpr bool test_all() const noexcept { return (... && test<comp>()); }

	template<std::integral... I>
	constexpr bool test_all(I...comp) const noexcept { return (... && test(comp)); }

	template<valid_enum... E>
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
		for (const auto ev : conjure_enum<T>::values)
			if (test(ev))
				std::invoke(std::forward<Fn>(func), ev, std::forward<Args>(args)...);
		return std::bind(std::forward<Fn>(func), std::placeholders::_1, std::forward<Args>(args)...);
	}

	template<typename C, typename Fn, typename... Args> // specialisation for member function with object
	requires std::invocable<Fn&&, C, T, Args...>
	[[maybe_unused]] constexpr auto for_each(Fn&& func, C *obj, Args&&... args) noexcept
	{
		return for_each(std::bind(std::forward<Fn>(func), obj, std::placeholders::_1, std::forward<Args>(args)...));
	}

	/// create a bitset from enum separated enum string
	static constexpr U factory(std::string_view src, bool anyscope, char sep, bool ignore_errors)
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
template<typename T>
class conjure_type
{
	static constexpr std::string_view _get_name() noexcept
	{
		constexpr std::string_view from{tpeek()};
		constexpr auto ep { from.rfind(cs::get_spec<0,1>()) };
		if constexpr (ep == std::string_view::npos)
			return {};
		if constexpr (from[ep + cs::get_spec<0,1>().size()] == cs::get_spec<3,1>())
		{
			if (constexpr auto lstr { from.substr(ep + cs::get_spec<0,1>().size()) }; lstr.find(cs::get_spec<2,1>()) != std::string_view::npos)	// is anon
				if constexpr (constexpr auto lc { lstr.find_first_of(cs::get_spec<1,1>()) }; lc != std::string_view::npos)
					return lstr.substr(cs::get_spec<2,1>().size() + 2, lc - (cs::get_spec<2,1>().size() + 2)); // eat "::"
		}
		constexpr std::string_view result { from.substr(ep + cs::get_spec<0,1>().size()) };
		if constexpr (constexpr auto lc { result.find_first_of(cs::get_spec<1,1>()) }; lc != std::string_view::npos)
			return result.substr(0, lc);
		return {};
	}
#if !defined _MSC_VER
	static constexpr auto _type_name() noexcept
	{
		constexpr auto result { _get_name() };
		return fixed_string<result.size()>(result);
	}
#endif

public:
	conjure_type() = delete;
	~conjure_type() = delete;
	conjure_type(const conjure_type&) = delete;
	conjure_type& operator=(const conjure_type&) = delete;
	conjure_type(conjure_type&&) = delete;
	conjure_type& operator=(conjure_type&&) = delete;

	static consteval const char *tpeek() noexcept { return std::source_location::current().function_name(); }
	static constexpr auto name
	{
#if defined _MSC_VER
		_get_name()
#else
		_type_name()
#endif
	};
	static constexpr std::string_view as_string_view() noexcept
	{
#if defined _MSC_VER
		return name;
#else
		return name.get();
#endif
	}
};

//-----------------------------------------------------------------------------------------
} // FIX8

#endif // FIX8_CONJURE_ENUM_HPP_

