//-----------------------------------------------------------------------------------------
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2024 Fix8 Market Technologies Pty Ltd
// SPDX-FileType: SOURCE
//
// conjure_enum (header only)
//   by David L. Dight
// see https://github.com/fix8mt/conjure_enum
//
// Lightweight header-only C++20 enum and typename reflection
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
// conjure_enum
//----------------------------------------------------------------------------------------
#ifndef FIX8_CONJURE_ENUM_HPP_
#define FIX8_CONJURE_ENUM_HPP_

#if defined _MSC_VER && (_MSC_VER < 1910) || !defined _MSC_VER && (__cplusplus < 202002L)
# error "conjure_enum requires C++20 support"
#endif

//----------------------------------------------------------------------------------------
#if defined FIX8_CONJURE_ENUM_ALL_OPTIMIZATIONS
# if not defined FIX8_CONJURE_ENUM_IS_CONTINUOUS
#  define FIX8_CONJURE_ENUM_IS_CONTINUOUS
# endif
# if not defined FIX8_CONJURE_ENUM_NO_ANON
#  define FIX8_CONJURE_ENUM_NO_ANON
# endif
# if not defined FIX8_CONJURE_ENUM_MINIMAL
#  define FIX8_CONJURE_ENUM_MINIMAL
# endif
#endif

//-----------------------------------------------------------------------------------------
#include <source_location>
#include <algorithm>
#include <string_view>
#include <tuple>
#include <concepts>
#include <optional>
#include <array>
#if not defined FIX8_CONJURE_ENUM_MINIMAL
# include <functional>
#endif

//-----------------------------------------------------------------------------------------
namespace FIX8 {

//-----------------------------------------------------------------------------------------
// global default enum range
//-----------------------------------------------------------------------------------------
#if not defined FIX8_CONJURE_ENUM_MIN_VALUE
# define FIX8_CONJURE_ENUM_MIN_VALUE -128
#endif
#if not defined FIX8_CONJURE_ENUM_MAX_VALUE
# define FIX8_CONJURE_ENUM_MAX_VALUE 127
#endif

//-----------------------------------------------------------------------------------------
template<std::size_t N>
class fixed_string final
{
	const std::array<char, N + 1> _buff;
	template<std::size_t... I>
	constexpr fixed_string(std::string_view sv, std::index_sequence<I...>) noexcept : _buff{sv[I]..., 0} {}

public:
	explicit constexpr fixed_string(std::string_view sv) noexcept : fixed_string{sv, std::make_index_sequence<N>{}} {}
	constexpr fixed_string() = delete;
	constexpr std::string_view get() const noexcept { return { _buff.data(), N }; }
	constexpr const char *c_str() const noexcept { return _buff.data(); }
	constexpr operator std::string_view() const noexcept { return get(); }
	constexpr char operator[](size_t idx) const noexcept { return _buff[idx]; }
	constexpr std::size_t size() const noexcept { return _buff.size(); }
	friend std::ostream& operator<<(std::ostream& os, const fixed_string& what) noexcept { return os << what.get(); }
};

//-----------------------------------------------------------------------------------------
class static_only
{
protected:
	static_only() = delete;
#if defined _MSC_VER
	~static_only() = default; // warning C4624
#else
	~static_only() = delete;
#endif
	static_only(const static_only&) = delete;
	static_only& operator=(const static_only&) = delete;
	static_only(static_only&&) = delete;
	static_only& operator=(static_only&&) = delete;
};

//-----------------------------------------------------------------------------------------
// compiler specifics
//-----------------------------------------------------------------------------------------
class cs final : public static_only
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
			{ "epeek<", '>', "`anonymous-namespace'", '`' }, { "::tpeek", '<', "enum `anonymous namespace'::", '\0' },
			{ "", '\0', "`anonymous namespace'::", '\0' }, { "", '\0', "enum ", '\0' }, { "", '\0', "class ", '\0' },
			{ "", '\0', "struct ", '\0' },
#else
# error "conjure_enum not supported by your compiler"
#endif
		})
	};

public:
	enum class stype { enum_t, type_t, extype_t0, extype_t1, extype_t2, extype_t3 };
	enum class sval { start, end, anon_str, anon_start };

	template<sval N, stype V> // can't have constexpr decompositions! (but why not?)
	static constexpr auto get_spec() noexcept { return std::get<static_cast<int>(N)>(_specifics[static_cast<int>(V)]); }
	static constexpr auto size() noexcept { return sizeof(_specifics); }
};
using stype = cs::stype;
using sval = cs::sval;

#if defined _MSC_VER
#define CHKMSSTR(e, x) \
	if constexpr (constexpr auto ep##x { e.find(cs::get_spec<sval::anon_str,stype::x>()) }; ep##x != std::string_view::npos) \
		return e.substr(ep##x + cs::get_spec<sval::anon_str,stype::x>().size(), e.size() - (ep##x + cs::get_spec<sval::anon_str,stype::x>().size()))
#endif

//-----------------------------------------------------------------------------------------
template<typename T>
concept valid_enum = requires(T)
{
	requires std::same_as<T, std::decay_t<T>>;
	requires std::is_enum_v<T>;
};

//-----------------------------------------------------------------------------------------
// You can specialise this class to define a custom range for your enum
// Alternatively, alias T::ce_first as the first and T::ce_last as the last enum in
// your enum declaration
//-----------------------------------------------------------------------------------------
template<typename T>
class enum_range final : public static_only
{
	static constexpr int get_first() noexcept requires std::is_enum_v<decltype(T::ce_first)>
		{ return static_cast<int>(T::ce_first); }
	static constexpr int get_last() noexcept requires std::is_enum_v<decltype(T::ce_last)>
		{ return static_cast<int>(T::ce_last); }
	static constexpr int get_first() noexcept { return FIX8_CONJURE_ENUM_MIN_VALUE; }
	static constexpr int get_last() noexcept { return FIX8_CONJURE_ENUM_MAX_VALUE; };

public:
	static constexpr int min{get_first()}, max{get_last()};
};

//-----------------------------------------------------------------------------------------
// Convenience macros for above
//-----------------------------------------------------------------------------------------
#define FIX8_CONJURE_ENUM_SET_RANGE_INTS(ec,minv,maxv) \
	template<> struct FIX8::enum_range<ec> final { static constexpr int min{minv}, max{maxv}; };

#define FIX8_CONJURE_ENUM_SET_RANGE(minv,maxv) \
	FIX8_CONJURE_ENUM_SET_RANGE_INTS(decltype(minv),static_cast<int>(minv), static_cast<int>(maxv))

//-----------------------------------------------------------------------------------------
template<valid_enum T>
class conjure_enum : public static_only
{
	static constexpr int enum_min_value{enum_range<T>::min}, enum_max_value{enum_range<T>::max};
	static_assert(enum_max_value > enum_min_value,
		"FIX8_CONJURE_ENUM_MAX_VALUE, enum_range<T>::max or T::ce_first must be greater than FIX8_CONJURE_ENUM_MIN_VALUE, enum_range<T>::min or T::ce_last) ");

public:
	using enum_tuple = std::tuple<T, std::string_view>;
	using scoped_tuple = std::tuple<std::string_view, std::string_view>;

	static consteval const char *tpeek() noexcept { return std::source_location::current().function_name(); }

	template<T e>
	static consteval const char *epeek() noexcept { return std::source_location::current().function_name(); }

private:
	template<T e>
	static constexpr std::string_view _epeek_v { epeek<e>() };

	template<T e>
	static constexpr bool _is_valid() noexcept
	{
		if constexpr (constexpr auto ep { _epeek_v<e>.rfind(cs::get_spec<sval::start,stype::enum_t>()) }; ep == std::string_view::npos)
			return false;
#if defined __clang__
		else if constexpr (_epeek_v<e>[ep + cs::get_spec<sval::start,stype::enum_t>().size()] == '(')
		{
			if constexpr (_epeek_v<e>[ep + cs::get_spec<sval::start,stype::enum_t>().size() + 1] == '(')
				return false;
#if not defined FIX8_CONJURE_ENUM_NO_ANON
			if constexpr (_epeek_v<e>.find(cs::get_spec<sval::anon_str,stype::enum_t>(), ep + cs::get_spec<sval::start,stype::enum_t>().size()) != std::string_view::npos)	// is anon
				return true;
#endif
		}
		else if constexpr (_epeek_v<e>.find_first_of(cs::get_spec<sval::end,stype::enum_t>(), ep + cs::get_spec<sval::start,stype::enum_t>().size()) != std::string_view::npos)
			return true;
		return false;
#else
		else if constexpr (_epeek_v<e>[ep + cs::get_spec<sval::start,stype::enum_t>().size()] != '('
			&& _epeek_v<e>.find_first_of(cs::get_spec<sval::end,stype::enum_t>(), ep + cs::get_spec<sval::start,stype::enum_t>().size()) != std::string_view::npos)
				return true;
		else
			return false;
#endif
	}

	template<std::size_t... I>
	static constexpr auto _values(std::index_sequence<I...>) noexcept
	{
#if defined FIX8_CONJURE_ENUM_IS_CONTINUOUS
		static_assert(sizeof...(I) > 0, "conjure_enum requires non-empty enum");
		return std::array<T, sizeof...(I)>{static_cast<T>(enum_min_value + I)... };
#else
		constexpr std::array<bool, sizeof...(I)> valid { _is_valid<static_cast<T>(enum_min_value + I)>()... };
		constexpr auto valid_cnt { std::count_if(valid.cbegin(), valid.cend(), [](bool val) noexcept { return val; }) };
		static_assert(valid_cnt > 0, "conjure_enum requires non-empty enum");
		std::array<T, valid_cnt> vals{};
		for(std::size_t idx{}, nn{}; nn < valid_cnt; ++idx)
			if (valid[idx])
				vals[nn++] = static_cast<T>(enum_min_value + idx);
		return vals;
#endif
	}

	template<T e>
	static constexpr std::string_view _get_name() noexcept
	{
		constexpr auto ep { _epeek_v<e>.rfind(cs::get_spec<sval::start,stype::enum_t>()) };
		if constexpr (ep == std::string_view::npos)
			return {};
#if not defined FIX8_CONJURE_ENUM_NO_ANON
		if constexpr (_epeek_v<e>[ep + cs::get_spec<sval::start,stype::enum_t>().size()] == cs::get_spec<sval::anon_start,stype::enum_t>())
		{
#if defined __clang__
			if constexpr (_epeek_v<e>[ep + cs::get_spec<sval::start,stype::enum_t>().size() + 1] == cs::get_spec<sval::anon_start,stype::enum_t>())
				return {};
#endif
			if (constexpr auto lstr { _epeek_v<e>.substr(ep + cs::get_spec<sval::start,stype::enum_t>().size()) };
				lstr.find(cs::get_spec<sval::anon_str,stype::enum_t>()) != std::string_view::npos)	// is anon
					if constexpr (constexpr auto lc { lstr.find_first_of(cs::get_spec<sval::end,stype::enum_t>()) }; lc != std::string_view::npos)
						return lstr.substr(cs::get_spec<sval::anon_str,stype::enum_t>().size() + 2, lc - (cs::get_spec<sval::anon_str,stype::enum_t>().size() + 2)); // eat "::"
		}
#endif
		constexpr std::string_view result { _epeek_v<e>.substr(ep + cs::get_spec<sval::start,stype::enum_t>().size()) };
		if constexpr (constexpr auto lc { result.find_first_of(cs::get_spec<sval::end,stype::enum_t>()) }; lc != std::string_view::npos)
			return result.substr(0, lc);
		else
			return {};
	}

	template<T e>
	static constexpr auto _get_name_v { _get_name<e>() };

	template<T e>
	static constexpr auto _enum_name_v { fixed_string<_get_name_v<e>.size()>(_get_name_v<e>) };

	template<std::size_t... I>
	static constexpr auto _entries(std::index_sequence<I...>) noexcept
	{
		return std::array<enum_tuple, sizeof...(I)>{{{ values[I], _enum_name_v<values[I]>}...}};
	}

	static constexpr auto _sorted_entries() noexcept
	{
		auto tmp { entries };
		std::sort(tmp.begin(), tmp.end(), _tuple_comp_rev);
		return tmp;
	}

	/// comparators
	static constexpr bool _value_comp(const T& pl, const T& pr) noexcept
	{
		return pl < pr;
	}
	static constexpr bool _tuple_comp(const enum_tuple& pl, const enum_tuple& pr) noexcept
	{
		return std::get<T>(pl) < std::get<T>(pr);
	}
	static constexpr bool _tuple_comp_rev(const enum_tuple& pl, const enum_tuple& pr) noexcept
	{
		return std::get<std::string_view>(pl) < std::get<std::string_view>(pr);
	}

public:
	struct is_scoped : std::bool_constant<requires
	{
		requires !std::convertible_to<T, std::underlying_type_t<T>>;
	}>{};

	static constexpr auto count() noexcept { return values.size(); }
	static constexpr bool is_continuous() noexcept { return (static_cast<size_t>(max_v) - static_cast<size_t>(min_v) + 1) == count(); }
	static constexpr bool in_range(T value) noexcept { return std::clamp(value, min_v, max_v) == value; }

	// scope ops
	static constexpr bool has_scope(std::string_view what) noexcept
	{
		if constexpr (is_scoped())
			return contains(what);
		else
			return false;
	}

	// enum <==> int
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
		return contains(enum_cast(value)) ? enum_cast(value) : std::optional<T>{};
	}
	static constexpr T enum_cast(int value) noexcept
	{
		return static_cast<T>(value);
	}

	// index
	static constexpr std::optional<size_t> index(T value) noexcept
	{
		if constexpr (is_continuous())
			return in_range(value) ? enum_to_underlying(value) - enum_to_underlying(min_v) : std::optional<size_t>{};
		else
		{
			const auto [begin,end] { std::equal_range(entries.cbegin(), entries.cend(), enum_tuple(value, std::string_view()), _tuple_comp) };
			return begin != end ? &*begin - &*entries.cbegin() : std::optional<size_t>{};
		}
	}
	template<T e>
	static constexpr std::optional<size_t> index() noexcept { return index(e); }

	// contains
	template<T e>
	static constexpr bool is_valid() noexcept { return contains<e>(); }

	static constexpr bool contains(T value) noexcept
	{
		if constexpr (is_continuous())
			return in_range(value);
		else
			return std::binary_search(values.cbegin(), values.cend(), value, _value_comp);
   }
	static constexpr bool contains(std::string_view str) noexcept
	{
		return std::binary_search(sorted_entries.cbegin(), sorted_entries.cend(), enum_tuple(T{}, str), _tuple_comp_rev);
	}
	template<T e>
	static constexpr bool contains() noexcept { return contains(e); }

	// string <==> enum
	template<T e>
	static constexpr std::string_view enum_to_string() noexcept { return _get_name_v<e>; }

	static constexpr std::string_view enum_to_string(T value, [[maybe_unused]] bool noscope=false) noexcept
	{
		if constexpr (is_continuous())
		{
			if (in_range(value))
			{
#if not defined FIX8_CONJURE_ENUM_MINIMAL
				if (noscope)
					return remove_scope(std::get<std::string_view>(entries[enum_to_underlying(value)]));
#endif
				return std::get<std::string_view>(entries[enum_to_underlying(value)]);
			}
		}
		else if (const auto [begin,end] { std::equal_range(entries.cbegin(), entries.cend(), enum_tuple(value, std::string_view()), _tuple_comp) };
			begin != end)
		{
#if not defined FIX8_CONJURE_ENUM_MINIMAL
			if (noscope)
				return remove_scope(std::get<std::string_view>(*begin));
#endif
			return std::get<std::string_view>(*begin);
		}
		return {};
	}
	static constexpr std::optional<T> string_to_enum(std::string_view str) noexcept
	{
		const auto [begin,end] { std::equal_range(sorted_entries.cbegin(), sorted_entries.cend(), enum_tuple(T{}, str), _tuple_comp_rev) };
		return begin != end ? std::get<T>(*begin) : std::optional<T>{};
	}

	// public constexpr data structures
	static constexpr auto values { _values(std::make_index_sequence<enum_max_value - enum_min_value + 1>()) };
	static constexpr auto entries { _entries(std::make_index_sequence<count()>()) };
	static constexpr auto sorted_entries { _sorted_entries() };

	// misc
	static constexpr int get_enum_min_value() noexcept { return enum_min_value; }
	static constexpr int get_enum_max_value() noexcept { return enum_max_value; }
	static constexpr auto min_v { values.front() };
	static constexpr auto max_v { values.back() };
	static constexpr int get_actual_enum_min_value() noexcept { return static_cast<int>(min_v); }
	static constexpr int get_actual_enum_max_value() noexcept { return static_cast<int>(max_v); }

#if not defined FIX8_CONJURE_ENUM_MINIMAL
#include <fix8/conjure_enum_ext.hpp>
#else
};
#endif

//-----------------------------------------------------------------------------------------
} // FIX8

#endif // FIX8_CONJURE_ENUM_HPP_

