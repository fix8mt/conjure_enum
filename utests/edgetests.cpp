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
#include <fix8/conjure_enum.hpp>

//-----------------------------------------------------------------------------------------
class foobat{};

namespace
{
	enum class NineEnums : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
	enum NineEnums1 : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
	namespace TEST1
	{
		enum class NineEnums : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
		enum NineEnums1 : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
		class foo{};
	}
	class foo{};
}

namespace TEST
{
	enum class NineEnums : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
	enum NineEnums1 : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
	namespace TEST1
	{
		enum class NineEnums : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
		enum NineEnums1 : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
		class foo{};
	}
	class foo{};
}

namespace test::rsp::gd
{
	class AFrame
	{
	public:
		enum class PreFrames { one };
	};
}

namespace test::util
{
	template <typename E>
	[[nodiscard]] constexpr size_t countOf() { return FIX8::conjure_enum<E>::count(); }
}

//-----------------------------------------------------------------------------------------
using namespace FIX8;

//-----------------------------------------------------------------------------------------
// run as: ctest --output-on-failure
//-----------------------------------------------------------------------------------------
TEST_CASE("anonymous entries")
{
	static constexpr auto compentries
	{
      std::to_array<std::tuple<NineEnums, std::string_view>>
      ({
			{ NineEnums::One, "NineEnums::One" },
			{ NineEnums::Two, "NineEnums::Two" },
			{ NineEnums::Three, "NineEnums::Three" },
			{ NineEnums::Four, "NineEnums::Four" },
			{ NineEnums::Five, "NineEnums::Five" },
			{ NineEnums::Six, "NineEnums::Six" },
			{ NineEnums::Seven, "NineEnums::Seven" },
			{ NineEnums::Eight, "NineEnums::Eight" },
			{ NineEnums::Nine, "NineEnums::Nine" },
      })
   };
	static constexpr auto compentries1
	{
      std::to_array<std::tuple<NineEnums1, std::string_view>>
      ({
			{ One, "One" },
			{ Two, "Two" },
			{ Three, "Three" },
			{ Four, "Four" },
			{ Five, "Five" },
			{ Six, "Six" },
			{ Seven, "Seven" },
			{ Eight, "Eight" },
			{ Nine, "Nine" },
      })
   };
	REQUIRE(conjure_enum<NineEnums>::entries == compentries);
	REQUIRE(conjure_enum<NineEnums1>::entries == compentries1);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("namespaced entries")
{
	static constexpr auto compentries
	{
      std::to_array<std::tuple<TEST::NineEnums, std::string_view>>
      ({
			{ TEST::NineEnums::One, "TEST::NineEnums::One" },
			{ TEST::NineEnums::Two, "TEST::NineEnums::Two" },
			{ TEST::NineEnums::Three, "TEST::NineEnums::Three" },
			{ TEST::NineEnums::Four, "TEST::NineEnums::Four" },
			{ TEST::NineEnums::Five, "TEST::NineEnums::Five" },
			{ TEST::NineEnums::Six, "TEST::NineEnums::Six" },
			{ TEST::NineEnums::Seven, "TEST::NineEnums::Seven" },
			{ TEST::NineEnums::Eight, "TEST::NineEnums::Eight" },
			{ TEST::NineEnums::Nine, "TEST::NineEnums::Nine" },
      })
   };
	static constexpr auto compentries1
	{
      std::to_array<std::tuple<TEST::NineEnums1, std::string_view>>
      ({
			{ TEST::One, "TEST::One" },
			{ TEST::Two, "TEST::Two" },
			{ TEST::Three, "TEST::Three" },
			{ TEST::Four, "TEST::Four" },
			{ TEST::Five, "TEST::Five" },
			{ TEST::Six, "TEST::Six" },
			{ TEST::Seven, "TEST::Seven" },
			{ TEST::Eight, "TEST::Eight" },
			{ TEST::Nine, "TEST::Nine" },
      })
   };
	REQUIRE(conjure_enum<TEST::NineEnums>::entries == compentries);
	REQUIRE(conjure_enum<TEST::NineEnums1>::entries == compentries1);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("nested anonymous entries")
{
	static constexpr auto compentries
	{
      std::to_array<std::tuple<TEST1::NineEnums, std::string_view>>
      ({
			{ TEST1::NineEnums::One, "TEST1::NineEnums::One" },
			{ TEST1::NineEnums::Two, "TEST1::NineEnums::Two" },
			{ TEST1::NineEnums::Three, "TEST1::NineEnums::Three" },
			{ TEST1::NineEnums::Four, "TEST1::NineEnums::Four" },
			{ TEST1::NineEnums::Five, "TEST1::NineEnums::Five" },
			{ TEST1::NineEnums::Six, "TEST1::NineEnums::Six" },
			{ TEST1::NineEnums::Seven, "TEST1::NineEnums::Seven" },
			{ TEST1::NineEnums::Eight, "TEST1::NineEnums::Eight" },
			{ TEST1::NineEnums::Nine, "TEST1::NineEnums::Nine" },
      })
   };
	static constexpr auto compentries1
	{
      std::to_array<std::tuple<TEST1::NineEnums1, std::string_view>>
      ({
			{ TEST1::One, "TEST1::One" },
			{ TEST1::Two, "TEST1::Two" },
			{ TEST1::Three, "TEST1::Three" },
			{ TEST1::Four, "TEST1::Four" },
			{ TEST1::Five, "TEST1::Five" },
			{ TEST1::Six, "TEST1::Six" },
			{ TEST1::Seven, "TEST1::Seven" },
			{ TEST1::Eight, "TEST1::Eight" },
			{ TEST1::Nine, "TEST1::Nine" },
      })
   };
	REQUIRE(conjure_enum<TEST1::NineEnums>::entries == compentries);
	REQUIRE(conjure_enum<TEST1::NineEnums1>::entries == compentries1);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("nested namespaced entries")
{
	static constexpr auto compentries
	{
      std::to_array<std::tuple<TEST::TEST1::NineEnums, std::string_view>>
      ({
			{ TEST::TEST1::NineEnums::One, "TEST::TEST1::NineEnums::One" },
			{ TEST::TEST1::NineEnums::Two, "TEST::TEST1::NineEnums::Two" },
			{ TEST::TEST1::NineEnums::Three, "TEST::TEST1::NineEnums::Three" },
			{ TEST::TEST1::NineEnums::Four, "TEST::TEST1::NineEnums::Four" },
			{ TEST::TEST1::NineEnums::Five, "TEST::TEST1::NineEnums::Five" },
			{ TEST::TEST1::NineEnums::Six, "TEST::TEST1::NineEnums::Six" },
			{ TEST::TEST1::NineEnums::Seven, "TEST::TEST1::NineEnums::Seven" },
			{ TEST::TEST1::NineEnums::Eight, "TEST::TEST1::NineEnums::Eight" },
			{ TEST::TEST1::NineEnums::Nine, "TEST::TEST1::NineEnums::Nine" },
      })
   };
	static constexpr auto compentries1
	{
      std::to_array<std::tuple<TEST::TEST1::NineEnums1, std::string_view>>
      ({
			{ TEST::TEST1::One, "TEST::TEST1::One" },
			{ TEST::TEST1::Two, "TEST::TEST1::Two" },
			{ TEST::TEST1::Three, "TEST::TEST1::Three" },
			{ TEST::TEST1::Four, "TEST::TEST1::Four" },
			{ TEST::TEST1::Five, "TEST::TEST1::Five" },
			{ TEST::TEST1::Six, "TEST::TEST1::Six" },
			{ TEST::TEST1::Seven, "TEST::TEST1::Seven" },
			{ TEST::TEST1::Eight, "TEST::TEST1::Eight" },
			{ TEST::TEST1::Nine, "TEST::TEST1::Nine" },
      })
   };
	REQUIRE(conjure_enum<TEST::TEST1::NineEnums>::entries == compentries);
	REQUIRE(conjure_enum<TEST::TEST1::NineEnums1>::entries == compentries1);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("anonymous enum")
{
	REQUIRE(conjure_type<NineEnums>::as_string_view() == "NineEnums");
	REQUIRE(conjure_type<NineEnums1>::as_string_view() == "NineEnums1");
	REQUIRE(conjure_type<TEST::NineEnums>::as_string_view() == "TEST::NineEnums");
	REQUIRE(conjure_type<TEST::NineEnums1>::as_string_view() == "TEST::NineEnums1");
}

//-----------------------------------------------------------------------------------------
TEST_CASE("nested anonymous enum")
{
	REQUIRE(conjure_type<TEST1::NineEnums>::as_string_view() == "TEST1::NineEnums");
	REQUIRE(conjure_type<TEST1::NineEnums1>::as_string_view() == "TEST1::NineEnums1");
	REQUIRE(conjure_type<TEST::TEST1::NineEnums>::as_string_view() == "TEST::TEST1::NineEnums");
	REQUIRE(conjure_type<TEST::TEST1::NineEnums1>::as_string_view() == "TEST::TEST1::NineEnums1");
}

//-----------------------------------------------------------------------------------------
TEST_CASE("nested structured enum")
{
	REQUIRE(test::util::countOf<test::rsp::gd::AFrame::PreFrames>() == 1);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("anonymous type")
{
	REQUIRE(conjure_type<foobat>::as_string_view() == "foobat");
	REQUIRE(conjure_type<TEST::foo>::as_string_view() == "TEST::foo");
}

//-----------------------------------------------------------------------------------------
TEST_CASE("nested type")
{
	REQUIRE(conjure_type<TEST1::foo>::as_string_view() == "TEST1::foo");
	REQUIRE(conjure_type<TEST::TEST1::foo>::as_string_view() == "TEST::TEST1::foo");
}

