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
// Full build include fragment; do not include separately
//----------------------------------------------------------------------------------------
#ifndef FIX8_CONJURE_ENUM_EXT_HPP_
#define FIX8_CONJURE_ENUM_EXT_HPP_

//----------------------------------------------------------------------------------------
private:
	static constexpr bool _scoped_comp(const scoped_tuple& pl, const scoped_tuple& pr) noexcept
	{
		return std::get<0>(pl) < std::get<0>(pr);
	}

	template<std::size_t... I>
	static constexpr auto _names(std::index_sequence<I...>) noexcept
	{
		return std::array<std::string_view, sizeof...(I)>{{{ _enum_name_v<values[I]>}...}};
	}

	static constexpr std::string_view _remove_scope(std::string_view what) noexcept
	{
		if (const auto lc { what.find_last_of(':') }; lc != std::string_view::npos)
			return what.substr(lc + 1);
		return what;
	}

	template<std::size_t... I>
	static constexpr auto _unscoped_entries(std::index_sequence<I...>) noexcept
	{
		std::array<enum_tuple, sizeof...(I)> tmp{{{ values[I], _remove_scope(_enum_name_v<values[I]>)}...}};
		std::sort(tmp.begin(), tmp.end(), _tuple_comp_rev);
		return tmp;
	}

	template<std::size_t... I>
	static constexpr auto _scoped_entries(std::index_sequence<I...>) noexcept
	{
		std::array<scoped_tuple, sizeof...(I)> tmp{{{ _remove_scope(_enum_name_v<values[I]>), _enum_name_v<values[I]>}...}};
		std::sort(tmp.begin(), tmp.end(), _scoped_comp);
		return tmp;
	}

	template<std::size_t... I>
	static constexpr auto _rev_scoped_entries(std::index_sequence<I...>) noexcept
	{
		std::array<scoped_tuple, sizeof...(I)> tmp{{{ _enum_name_v<values[I]>, _remove_scope(_enum_name_v<values[I]>)}...}};
		std::sort(tmp.begin(), tmp.end(), _scoped_comp);
		return tmp;
	}

	template<std::size_t... I>
	static constexpr auto _unscoped_names(std::index_sequence<I...>) noexcept
	{
		return std::array<std::string_view, sizeof...(I)>{{{ _remove_scope(_enum_name_v<values[I]>)}...}};
	}

	static constexpr std::string_view _process_scope([[maybe_unused]] const auto& entr, std::string_view what) noexcept
	{
		if constexpr (is_scoped())
			if (const auto [begin,end] { std::equal_range(entr.cbegin(), entr.cend(), scoped_tuple(what, std::string_view()), _scoped_comp) };
				begin != end)
					return std::get<1>(*begin);
		return what;
	}

public:
	static constexpr std::string_view remove_scope(std::string_view what) noexcept
	{
		return _process_scope(rev_scoped_entries, what);
	}

	static constexpr std::string_view add_scope(std::string_view what) noexcept
	{
		return _process_scope(scoped_entries, what);
	}

	// iterators
	static constexpr auto cbegin() noexcept { return entries.cbegin(); }
	static constexpr auto cend() noexcept { return entries.cend(); }
	static constexpr auto crbegin() noexcept { return entries.crbegin(); }
	static constexpr auto crend() noexcept { return entries.crend(); }
	static constexpr auto front() noexcept { return *cbegin(); }
	static constexpr auto back() noexcept { return *std::prev(cend()); }
	static constexpr std::optional<T> unscoped_string_to_enum(std::string_view str) noexcept
	{
		const auto [begin,end] { std::equal_range(unscoped_entries.cbegin(), unscoped_entries.cend(), enum_tuple(T{}, str), _tuple_comp_rev) };
		return begin != end ? std::get<T>(*begin) : std::optional<T>{};
	}

	static constexpr std::string_view type_name() noexcept
	{
		constexpr std::string_view from{tpeek()};
#if defined _MSC_VER
		constexpr auto ep { from.rfind(cs::get_spec<sval::start,stype::type_t>()) };
		if constexpr (ep == std::string_view::npos)
			return {};
		if constexpr (constexpr auto lc { from.find_first_of(cs::get_spec<sval::end,stype::type_t>()) }; lc != std::string_view::npos)
		{
			constexpr auto e1 { from.substr(lc + 1, ep - lc - 2) };
			CHKMSSTR(e1,type_t);
			CHKMSSTR(e1,extype_t1);
		}
		else
			return {};
#else
		if constexpr (constexpr auto ep { from.rfind(cs::get_spec<sval::start,stype::type_t>()) }; ep != std::string_view::npos)
		{
			constexpr auto result { from.substr(ep + cs::get_spec<sval::start,stype::type_t>().size()) };
			if constexpr (constexpr auto lc { result.find_first_of(cs::get_spec<sval::end,stype::type_t>()) }; lc != std::string_view::npos)
				return result.substr(0, lc);
		}
		else
			return {};
#endif
	}

	/// for_each, for_each_n
	template<typename Fn, typename... Args>
	requires std::invocable<Fn&&, T, Args...>
	[[maybe_unused]] static constexpr auto for_each(Fn&& func, Args&&... args) noexcept
	{
		return for_each_n(static_cast<int>(count()), std::forward<Fn>(func), std::forward<Args>(args)...);
	}

	template<typename Fn, typename C, typename... Args> // specialisation for member function with object
	requires std::invocable<Fn&&, C, T, Args...>
	[[maybe_unused]] static constexpr auto for_each(Fn&& func, C *obj, Args&&... args) noexcept
	{
		return for_each(std::bind(std::forward<Fn>(func), obj, std::placeholders::_1, std::forward<Args>(args)...));
	}

	template<typename Fn, typename... Args>
	requires std::invocable<Fn&&, T, Args...>
	[[maybe_unused]] static constexpr auto for_each_n(int n, Fn&& func, Args&&... args) noexcept
	{
		for (int ii{}; const auto ev : conjure_enum<T>::values)
		{
			if (ii++ >= n)
				break;
			std::invoke(std::forward<Fn>(func), ev, std::forward<Args>(args)...);
		}
		return std::bind(std::forward<Fn>(func), std::placeholders::_1, std::forward<Args>(args)...);
	}

	template<typename Fn, typename C, typename... Args> // specialisation for member function with object
	requires std::invocable<Fn&&, C, T, Args...>
	[[maybe_unused]] static constexpr auto for_each_n(int n, Fn&& func, C *obj, Args&&... args) noexcept
	{
		return for_each_n(n, std::bind(std::forward<Fn>(func), obj, std::placeholders::_1, std::forward<Args>(args)...));
	}

	// dispatch
	template<typename Fn>
	static constexpr bool tuple_comp(const std::tuple<T, Fn>& pl, const std::tuple<T, Fn>& pr) noexcept
	{
		return std::get<T>(pl) < std::get<T>(pr);
	}

	template<std::size_t I, typename R, typename Fn, typename... Args> // with not found value(nval) for return
	requires std::invocable<Fn&&, T, Args...>
	[[maybe_unused]] static constexpr R dispatch(T ev, R nval, const std::array<std::tuple<T, Fn>, I>& disp, Args&&... args) noexcept
	{
		const auto [begin,end] { std::equal_range(disp.cbegin(), disp.cend(), std::make_tuple(ev, Fn()), tuple_comp<Fn>) };
		return begin != end ? std::invoke(std::get<Fn>(*begin), ev, std::forward<Args>(args)...) : nval;
	}

	template<std::size_t I, typename R, typename Fn, typename C, typename... Args> // specialisation for member function with not found value(nval) for return
	requires std::invocable<Fn&&, C, T, Args...>
	[[maybe_unused]] static constexpr R dispatch(T ev, R nval, const std::array<std::tuple<T, Fn>, I>& disp, C *obj, Args&&... args) noexcept
	{
		const auto [begin,end] { std::equal_range(disp.cbegin(), disp.cend(), std::make_tuple(ev, Fn()), tuple_comp<Fn>) };
		return begin != end ? std::invoke(std::get<Fn>(*begin), obj, ev, std::forward<Args>(args)...) : nval;
	}

	template<std::size_t I, typename Fn, typename... Args> // void func with not found call to last element
	requires (std::invocable<Fn&&, T, Args...> && I > 0)
	static constexpr void dispatch(T ev, const std::array<std::tuple<T, Fn>, I>& disp, Args&&... args) noexcept
	{
		const auto [begin,end] { std::equal_range(disp.cbegin(), std::prev(disp.cend()), std::make_tuple(ev, Fn()), tuple_comp<Fn>) };
		return std::invoke(std::get<Fn>(begin != end ? *begin : *std::prev(disp.cend())), ev, std::forward<Args>(args)...);
	}

	template<std::size_t I, typename Fn, typename C, typename... Args> // specialisation for void member function with not found call to last element
	requires (std::invocable<Fn&&, C, T, Args...> && I > 0)
	static constexpr void dispatch(T ev, const std::array<std::tuple<T, Fn>, I>& disp, C *obj, Args&&... args) noexcept
	{
		const auto [begin,end] { std::equal_range(disp.cbegin(), std::prev(disp.cend()), std::make_tuple(ev, Fn()), tuple_comp<Fn>) };
		return std::invoke(std::get<Fn>(begin != end ? *begin : *std::prev(disp.cend())), obj, ev, std::forward<Args>(args)...);
	}

	// public constexpr data structures
	static constexpr auto names { _names(std::make_index_sequence<count()>()) };
	static constexpr auto scoped_entries { _scoped_entries(std::make_index_sequence<count()>()) };
	static constexpr auto unscoped_entries { _unscoped_entries(std::make_index_sequence<count()>()) };
	static constexpr auto rev_scoped_entries { _rev_scoped_entries(std::make_index_sequence<count()>()) };
	static constexpr auto unscoped_names { _unscoped_names(std::make_index_sequence<count()>()) };
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
#include <ostream>

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

#endif // FIX8_CONJURE_ENUM_EXT_HPP_

