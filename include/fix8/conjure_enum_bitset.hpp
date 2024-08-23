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
// enum_bitset
//----------------------------------------------------------------------------------------
#ifndef FIX8_CONJURE_ENUM_BITSET_HPP_
#define FIX8_CONJURE_ENUM_BITSET_HPP_

//----------------------------------------------------------------------------------------
#include <bitset>
#include <exception>
#include <bit>

//-----------------------------------------------------------------------------------------
namespace FIX8 {

//-----------------------------------------------------------------------------------------
template<typename T>
concept valid_bitset_enum = valid_enum<T> and requires(T)
{
	requires conjure_enum<T>::is_continuous();
	requires conjure_enum<T>::get_actual_enum_min_value() == 0;
	requires conjure_enum<T>::get_actual_enum_max_value() < conjure_enum<T>::count();
	requires std::bit_ceil(static_cast<unsigned>(conjure_enum<T>::get_actual_enum_max_value())) >= conjure_enum<T>::count();
};

//-----------------------------------------------------------------------------------------
// bitset based on supplied enum
// Note: your enum sequence must be 0 based, continuous and the last enum value < count of enumerations
//-----------------------------------------------------------------------------------------
template<valid_bitset_enum T>
class enum_bitset
{
	static constexpr auto countof { conjure_enum<T>::count() };
	using U = std::conditional_t<countof <= 8,  std::uint_least8_t,
				 std::conditional_t<countof <= 16, std::uint_least16_t,
				 std::conditional_t<countof <= 32, std::uint_least32_t,
				 std::conditional_t<countof <= 64, std::uint_least64_t, void>>>>;
	static_assert(std::integral<U>, "requested bitset overflow");

	template <typename R>
	class _reference
	{
		R& _owner;
		U _idx;
		constexpr _reference(R& obj, U idx) noexcept : _owner(obj), _idx(idx) {}
		friend class enum_bitset;

	public:
		constexpr _reference& operator=(bool val) noexcept
		{
			if (val)
				_owner.set(_idx);
			else
				_owner.reset(_idx);
			return *this;
		}

		constexpr _reference& operator=(const _reference& val) noexcept
		{
			if (this == &val)
				return *this;
			*this = static_cast<bool>(val);
			return *this;
		}

		constexpr operator bool() const noexcept { return static_cast<bool>(_owner.test(_idx)); }
	};

	template<T val>
	static constexpr U to_underlying() noexcept { return static_cast<U>(val); } // C++23: upgrade to std::to_underlying
	static constexpr U to_underlying(T val) noexcept { return static_cast<U>(val); }
	static constexpr U all_bits { (1 << countof) - 1 };
	U _present{};

public:
	using enum_bitset_underlying_type = U;
	using reference = _reference<enum_bitset>;
	using const_reference = _reference<const enum_bitset>;

	explicit constexpr enum_bitset(U bits) noexcept : _present(bits) {}
	constexpr enum_bitset(std::string_view from, bool anyscope=false, char sep='|', bool ignore_errors=true)
		: _present(factory(from, anyscope, sep, ignore_errors)) {}

	template<valid_bitset_enum... E>
	requires (sizeof...(E) > 1)
	constexpr enum_bitset(E... comp) noexcept : _present((0u | ... | (1 << to_underlying(comp)))) {}

	template<std::integral... I>
	requires (sizeof...(I) > 1)
	constexpr enum_bitset(I... comp) noexcept : _present((0u | ... | (1 << comp))) {}

	constexpr enum_bitset() = default;
	constexpr ~enum_bitset() = default;

	constexpr std::size_t count() const noexcept
		{ return std::popcount(static_cast<std::make_unsigned_t<U>>(_present)); } // C++23: upgrade to std::bitset when count is constexpr
	constexpr std::size_t not_count() const noexcept { return countof - count(); }
	constexpr std::size_t size() const noexcept { return countof; }
	constexpr U to_ulong() const noexcept { return _present; }
	constexpr unsigned long long to_ullong() const noexcept { return _present; }

	constexpr auto operator[](U pos) noexcept { return reference(*this, pos); }
	constexpr auto operator[](U pos) const noexcept { return const_reference(*this, pos); }
	constexpr auto operator[](T what) noexcept { return reference(*this, to_underlying(what)); }
	constexpr auto operator[](T what) const noexcept { return const_reference(*this, to_underlying(what)); }

	/// set
	constexpr void set(U pos, bool value=true) noexcept { value ? _present |= 1 << pos : _present &= ~(1 << pos); }
	constexpr void set(T what, bool value=true) noexcept { set(to_underlying(what), value); }
	constexpr void set() noexcept { _present = all_bits; }

	template<T what>
	constexpr void set() noexcept
	{
		if constexpr (constexpr auto uu{to_underlying<what>()}; uu < countof)
			_present |= 1 << uu;
	}

	template<T... comp>
	requires (sizeof...(comp) > 1)
	constexpr void set() noexcept { (set<comp>(),...); }

	template<valid_bitset_enum... E>
	requires (sizeof...(E) > 1)
	constexpr void set(E... comp) noexcept { return (... | (set(comp))); }

	/// flip
	template<T what>
	constexpr void flip() noexcept
	{
		if constexpr (constexpr auto uu{to_underlying<what>()}; uu < countof)
			_present ^= 1 << uu;
	}

	constexpr void flip() noexcept { _present = ~_present & all_bits; }
	constexpr void flip(U pos) noexcept { _present ^= 1 << pos; }
	constexpr void flip(T what) noexcept { flip(to_underlying(what)); }

	/// reset
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
	requires (sizeof...(comp) > 1)
	constexpr void reset() noexcept { (reset<comp>(),...); }

	template<std::integral... I>
	constexpr void reset(I...comp) noexcept { (reset(comp),...); }

	/// test
	constexpr bool test(U pos) const noexcept { return _present & 1 << pos; }
	constexpr bool test(T what) const noexcept { return test(to_underlying(what)); }
	constexpr bool test() const noexcept { return _present; }

	template<T what>
	constexpr bool test() const noexcept
	{
		if constexpr (constexpr auto uu{to_underlying<what>()}; uu < countof)
			return test(uu);
		else
			return false;
	}

	template<T... comp>
	constexpr bool any_of() const noexcept { return (... || test<comp>()); }

	template<std::integral... I>
	constexpr bool any_of(I...comp) const noexcept { return (... || test(U(comp))); }

	template<valid_bitset_enum... E>
	constexpr bool any_of(E...comp) const noexcept { return (... || test(U(comp))); }

	template<T... comp>
	constexpr bool all_of() const noexcept { return (... && test<comp>()); }

	template<std::integral... I>
	constexpr bool all_of(I...comp) const noexcept { return (... && test(U(comp))); }

	template<valid_bitset_enum... E>
	constexpr bool all_of(E...comp) const noexcept { return (... && test(U(comp))); }

	template<T... comp>
	constexpr bool none_of() const noexcept { return (... && !test<comp>()); }

	template<std::integral... I>
	constexpr bool none_of(I...comp) const noexcept { return (... && !test(U(comp))); }

	template<valid_bitset_enum... E>
	constexpr bool none_of(E...comp) const noexcept { return (... && !test(U(comp))); }

	constexpr bool any() const noexcept { return count(); }
	constexpr bool all() const noexcept { return _present == all_bits; }
	constexpr bool none() const noexcept { return !*this; }

	/// operators
	constexpr operator bool() const noexcept { return count(); }
	constexpr enum_bitset& operator<<=(std::size_t pos) noexcept { _present <<= pos; return *this; }
	constexpr enum_bitset& operator>>=(std::size_t pos) noexcept { _present >>= pos; return *this; }
	constexpr enum_bitset& operator&=(T other) noexcept { _present &= 1 << to_underlying(other); return *this; }
	constexpr enum_bitset& operator|=(T other) noexcept { _present |= 1 << to_underlying(other); return *this; }
	constexpr enum_bitset& operator^=(T other) noexcept { _present ^= 1 << to_underlying(other); return *this; }
	constexpr enum_bitset& operator&=(U other) noexcept { _present &= other; return *this; }
	constexpr enum_bitset& operator|=(U other) noexcept { _present |= other; return *this; }
	constexpr enum_bitset& operator^=(U other) noexcept { _present ^= other; return *this; }

	constexpr enum_bitset operator<<(int pos) const noexcept { return enum_bitset(_present << pos); }
	constexpr enum_bitset operator>>(int pos) const noexcept { return enum_bitset(_present >> pos); }
	constexpr enum_bitset operator&(T other) const noexcept { return enum_bitset(_present & 1 << to_underlying(other)); }
	constexpr enum_bitset operator|(T other) const noexcept { return enum_bitset(_present | 1 << to_underlying(other)); }
	constexpr enum_bitset operator^(T other) const noexcept { return enum_bitset(_present ^ 1 << to_underlying(other)); }
	//constexpr enum_bitset operator&(U other) const noexcept { return enum_bitset(_present & other); }
	//constexpr enum_bitset operator|(U other) const noexcept { return enum_bitset(_present | other); }
	//constexpr enum_bitset operator^(U other) const noexcept { return enum_bitset(_present ^ other); }
	constexpr enum_bitset operator~() const noexcept { return enum_bitset(~_present & all_bits); }

	/// for_each, for_each_n
	template<typename Fn, typename... Args>
	requires std::invocable<Fn&&, T, Args...>
	[[maybe_unused]] constexpr auto for_each(Fn&& func, Args&&... args) noexcept
	{
		return for_each_n(static_cast<int>(countof), std::forward<Fn>(func), std::forward<Args>(args)...);
	}

	template<typename C, typename Fn, typename... Args> // specialisation for member function with object
	requires std::invocable<Fn&&, C, T, Args...>
	[[maybe_unused]] constexpr auto for_each(Fn&& func, C *obj, Args&&... args) noexcept
	{
		return for_each(std::bind(std::forward<Fn>(func), obj, std::placeholders::_1, std::forward<Args>(args)...));
	}

	template<typename Fn, typename... Args>
	requires std::invocable<Fn&&, T, Args...>
	[[maybe_unused]] constexpr auto for_each_n(int n, Fn&& func, Args&&... args) noexcept
	{
		for (int ii{}, jj{}; ii < static_cast<int>(countof) && jj < n; ++ii)
			if (const auto ev{conjure_enum<T>::values[ii]}; test(ev))
				std::invoke(std::forward<Fn>(func), ev, std::forward<Args>(args)...), ++jj;
		return std::bind(std::forward<Fn>(func), std::placeholders::_1, std::forward<Args>(args)...);
	}

	template<typename C, typename Fn, typename... Args> // specialisation for member function with object
	requires std::invocable<Fn&&, C, T, Args...>
	[[maybe_unused]] constexpr auto for_each_n(int n, Fn&& func, C *obj, Args&&... args) noexcept
	{
		return for_each_n(n, std::bind(std::forward<Fn>(func), obj, std::placeholders::_1, std::forward<Args>(args)...));
	}

	/// create a bitset from custom separated enum string
	static constexpr U factory(std::string_view src, bool anyscope, char sep, bool ignore_errors)
	{
		enum_bitset result;
		constexpr auto trim([](std::string_view src) noexcept ->auto
		{
		   const auto bg(src.find_first_not_of(" \t"));
			return bg == std::string_view::npos ? src : src.substr(bg, src.find_last_not_of(" \t") - bg + 1);
		});
		auto process([anyscope,&result](std::string_view src) noexcept ->auto
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

#endif // FIX8_CONJURE_ENUM_BITSET_HPP_

