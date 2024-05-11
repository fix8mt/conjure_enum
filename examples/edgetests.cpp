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
#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <iostream>
#include <fix8/conjure_enum.hpp>

//-----------------------------------------------------------------------------------------
using namespace FIX8;
using namespace std::literals::string_view_literals;
using namespace std::literals::string_literals;

//-----------------------------------------------------------------------------------------
namespace
{
	enum class NineEnums { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
	enum NineEnums1 { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
}

//-----------------------------------------------------------------------------------------
// run as: ctest --output-on-failure
//-----------------------------------------------------------------------------------------
TEST_CASE("anonymous namespace")
{
}
