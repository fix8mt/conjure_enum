//-----------------------------------------------------------------------------------------
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2024 Fix8 Market Technologies Pty Ltd
// SPDX-FileType: SOURCE

// conjure_enum (header only)
//   by David L. Dight
// see https://github.com/fix8mt/conjure_enum
//
// Lightweight header-only C++20 enum and type reflection
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
#include <iostream>
#include <fix8/conjure_enum.hpp>

//-----------------------------------------------------------------------------------------
enum class component : int { scheme, authority, userinfo, user, password, host, port, path, query, fragment };

namespace TEST
{
	enum class NineEnums : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
	enum NineEnums1 : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
	namespace TEST1
	{
		enum class NineEnums : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
		enum NineEnums1 : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
		namespace TEST2::TEST3::TEST4::TEST5
		{
			enum class NineEnums : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
			enum NineEnums1 : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
		}
	}
}

namespace gsdsp::dsp::fd
{

class AnalysisFrame
{
 public:
  enum class PrecomputedFrames : int
  { one };
};
}

namespace gsdsp::utilities
{
	template <typename E>
	[[nodiscard]]
	constexpr size_t countOf()
	{
	  return FIX8::conjure_enum<E>::count();
	}
}

//-----------------------------------------------------------------------------------------
using namespace FIX8;

//-----------------------------------------------------------------------------------------
int main(void)
{
	for(const auto& [a, b] : conjure_enum<component>::entries)
		std::cout << conjure_enum<component>::enum_to_int(a) << ' ' << b << '\n';
	for(const auto& [a, b] : conjure_enum<component>::unscoped_entries)
		std::cout << conjure_enum<component>::enum_to_int(a) << ' ' << b << '\n';
	for(const auto& a : conjure_enum<component>::names)
		std::cout << a << '\n';
	for(const auto& a : conjure_enum<component>::unscoped_names)
		std::cout << a << '\n';

	for(const auto& [a, b] : conjure_enum<TEST::TEST1::TEST2::TEST3::TEST4::TEST5::NineEnums>::entries)
		std::cout << conjure_enum<TEST::TEST1::TEST2::TEST3::TEST4::TEST5::NineEnums>::enum_to_int(a) << ' ' << b << '\n';
	std::cout << conjure_enum<TEST::TEST1::TEST2::TEST3::TEST4::TEST5::NineEnums>::count() << '\n';

	std::cout << gsdsp::utilities::countOf<gsdsp::dsp::fd::AnalysisFrame::PrecomputedFrames>() << '\n';
	return 0;
}
