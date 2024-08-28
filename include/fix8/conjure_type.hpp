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
// conjure_type
//----------------------------------------------------------------------------------------
#ifndef FIX8_CONJURE_TYPE_HPP_
#define FIX8_CONJURE_TYPE_HPP_

//-----------------------------------------------------------------------------------------
namespace FIX8 {

//-----------------------------------------------------------------------------------------
// General purpose class allowing you to extract a string representation of any typename.
// The string will be stored statically by the compiler, so you can use the statically generated value `name` to obtain your type.
//-----------------------------------------------------------------------------------------
template<typename T>
class conjure_type : public static_only
{
	static constexpr std::string_view _get_name() noexcept
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
			CHKMSSTR(e1,extype_t0);
			CHKMSSTR(e1,extype_t1);
			CHKMSSTR(e1,extype_t2);
			CHKMSSTR(e1,extype_t3);
		}
		return {};
	}
#else
		constexpr auto ep { from.rfind(cs::get_spec<sval::start,stype::type_t>()) };
		if constexpr (ep == std::string_view::npos)
			return {};
		if constexpr (from[ep + cs::get_spec<sval::start,stype::type_t>().size()] == cs::get_spec<sval::anon_start,stype::type_t>())
			if (constexpr auto lstr { from.substr(ep + cs::get_spec<sval::start,stype::type_t>().size()) };
				lstr.find(cs::get_spec<sval::anon_str,stype::type_t>()) != std::string_view::npos)	// is anon
					if constexpr (constexpr auto lc { lstr.find_first_of(cs::get_spec<sval::end,stype::type_t>()) }; lc != std::string_view::npos)
						return lstr.substr(cs::get_spec<sval::anon_str,stype::type_t>().size() + 2, lc - (cs::get_spec<sval::anon_str,stype::type_t>().size() + 2)); // eat "::"
		constexpr auto result { from.substr(ep + cs::get_spec<sval::start,stype::type_t>().size()) };
		if constexpr (constexpr auto lc { result.find_first_of(cs::get_spec<sval::end,stype::type_t>()) }; lc != std::string_view::npos)
			return result.substr(0, lc);
		return {};
	}
	static constexpr auto _type_name() noexcept
	{
		constexpr auto result { _get_name() };
		return fixed_string<result.size()>(result);
	}
#endif

public:
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

#endif // FIX8_CONJURE_TYPE_HPP_

