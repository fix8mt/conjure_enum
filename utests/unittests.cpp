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
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <string_view>
#include <iostream>
#include <sstream>
#include <fix8/conjure_enum.hpp>

//-----------------------------------------------------------------------------------------
using namespace FIX8;
using namespace std::literals::string_view_literals;
using namespace std::literals::string_literals;

//-----------------------------------------------------------------------------------------
enum class component : int { scheme, authority, userinfo, user, password, host, port, path=12, test=path, query, fragment };
enum component1 : int { scheme, authority, userinfo, user, password, host, port, path=12, query, fragment };
enum class numbers : int { zero, one, two, three, four, five, FIVE=five, six, seven, eight, nine };

//-----------------------------------------------------------------------------------------
// run as: ctest --output-on-failure
//-----------------------------------------------------------------------------------------
TEST_CASE("fixed_string")
{
	static constexpr std::string_view t1{"The rain in Spain"};
	fixed_string<t1.size()> f1{t1};
	REQUIRE(f1.size() == t1.size() + 1); // fixed_string makes string ASCIIZ
	REQUIRE(f1[t1.size()] == 0); // test ASCIIZ
	REQUIRE(f1.get().size() == t1.size()); // fixed_string as string_view
	REQUIRE(static_cast<std::string_view>(f1) == t1); // fixed_string as string_view
	REQUIRE(static_cast<std::string_view>(f1).size() == t1.size()); // fixed_string as string_view
}

//-----------------------------------------------------------------------------------------
TEST_CASE("is_valid")
{
	REQUIRE(conjure_enum<component>::is_valid<component::password>());
	REQUIRE(!conjure_enum<component>::is_valid<static_cast<component>(100)>());
	REQUIRE(conjure_enum<component1>::is_valid<password>());
	REQUIRE(!conjure_enum<component1>::is_valid<static_cast<component1>(100)>());
}

//-----------------------------------------------------------------------------------------
TEST_CASE("is_scoped")
{
	REQUIRE(conjure_enum<component>::is_scoped());
	REQUIRE(!conjure_enum<component1>::is_scoped());
}

//-----------------------------------------------------------------------------------------
TEST_CASE("count")
{
	REQUIRE(conjure_enum<component>::count() == 10);
	REQUIRE(conjure_enum<component1>::count() == 10);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("names")
{
	static constexpr auto compnames
	{
      std::to_array<std::string_view>
      ({
			"component::scheme", "component::authority", "component::userinfo", "component::user", "component::password",
			"component::host", "component::port", "component::path", "component::query", "component::fragment"
      })
   };
	static constexpr auto compnames1
	{
      std::to_array<std::string_view>
      ({
			"scheme", "authority", "userinfo", "user", "password", "host", "port", "path", "query", "fragment"
      })
   };
	REQUIRE(conjure_enum<component>::names == compnames);
	REQUIRE(conjure_enum<component1>::names == compnames1);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("unscoped_names")
{
	static constexpr auto compnames_both
	{
      std::to_array<std::string_view>
      ({
			"scheme", "authority", "userinfo", "user", "password", "host", "port", "path", "query", "fragment"
      })
   };
	REQUIRE(conjure_enum<component>::unscoped_names == compnames_both);
	REQUIRE(conjure_enum<component1>::unscoped_names == compnames_both);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("values")
{
	static constexpr std::array compvalues
	{
		component::scheme, component::authority, component::userinfo, component::user, component::password,
		component::host, component::port, component::path, component::query, component::fragment
	};
	static constexpr std::array compvalues1
	{
		scheme, authority, userinfo, user, password, host, port, path, query, fragment
	};
	REQUIRE(conjure_enum<component>::values == compvalues);
	REQUIRE(conjure_enum<component1>::values == compvalues1);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("entries")
{
	static constexpr auto compentries
	{
      std::to_array<std::tuple<component, std::string_view>>
      ({
			{ component::scheme, "component::scheme" },
			{ component::authority, "component::authority" },
			{ component::userinfo, "component::userinfo" },
			{ component::user, "component::user" },
			{ component::password, "component::password" },
			{ component::host, "component::host" },
			{ component::port, "component::port" },
			{ component::path, "component::path" },
			{ component::query, "component::query" },
			{ component::fragment, "component::fragment" }
      })
   };
	static constexpr auto compentries1
	{
      std::to_array<std::tuple<component1, std::string_view>>
      ({
			{ scheme, "scheme" },
			{ authority, "authority" },
			{ userinfo, "userinfo" },
			{ user, "user" },
			{ password, "password" },
			{ host, "host" },
			{ port, "port" },
			{ path, "path" },
			{ query, "query" },
			{ fragment, "fragment" }
      })
   };
	REQUIRE(conjure_enum<component>::entries == compentries);
	REQUIRE(conjure_enum<component1>::entries == compentries1);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("contains")
{
	REQUIRE(conjure_enum<component>::contains(component::path));
	REQUIRE(conjure_enum<component>::contains(component::test)); // alias
	REQUIRE(conjure_enum<component1>::contains(path));
	REQUIRE(!conjure_enum<component>::contains(static_cast<component>(100)));
	REQUIRE(conjure_enum<component>::contains("component::path"sv));
	REQUIRE(conjure_enum<component1>::contains("path"sv));
}

//-----------------------------------------------------------------------------------------
TEST_CASE("enum_to_string")
{
	REQUIRE(conjure_enum<component>::enum_to_string(component::path) == "component::path");
	REQUIRE(conjure_enum<component>::enum_to_string(component::test) == "component::path"); // alias
	REQUIRE(conjure_enum<component1>::enum_to_string(path) == "path");
	REQUIRE(conjure_enum<component>::enum_to_string(static_cast<component>(100)).empty());
	REQUIRE(conjure_enum<component>::enum_to_string<component::fragment>() == "component::fragment");
	REQUIRE(conjure_enum<component1>::enum_to_string<component1::fragment>() == "fragment");
}

//-----------------------------------------------------------------------------------------
TEST_CASE("remove_scope")
{
	using comp0 = conjure_enum<component>;
	REQUIRE(comp0::remove_scope(comp0::enum_to_string<component::fragment>()) == "fragment"sv);
	using comp1 = conjure_enum<component1>;
	REQUIRE(comp1::remove_scope(comp1::enum_to_string<fragment>()) == "fragment"sv);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("add_scope")
{
	REQUIRE(conjure_enum<component>::add_scope("path"sv) == "component::path"sv);
	REQUIRE(conjure_enum<component>::add_scope("component::path"sv) == "component::path"sv);
	REQUIRE(conjure_enum<component1>::add_scope("path"sv) == "path"sv);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("iterators")
{
	auto itr{conjure_enum<component>::cbegin()};
	const auto [a, b] {*itr};
	REQUIRE(a == component::scheme);
	REQUIRE(b == "component::scheme"sv);
	int cnt{};
	for (; itr != conjure_enum<component>::cend(); ++itr)
		++cnt;
	REQUIRE(cnt == conjure_enum<component>::count());
	REQUIRE(std::get<component>(conjure_enum<component>::back()) == component::fragment);
	REQUIRE(std::get<component>(conjure_enum<component>::front()) == component::scheme);
	REQUIRE(std::get<component>(conjure_enum<component>::back()) == std::get<component>(*conjure_enum<component>::crbegin()));
	REQUIRE(std::get<component1>(conjure_enum<component1>::front()) == scheme);
	REQUIRE(std::get<component1>(conjure_enum<component1>::back()) == fragment);
	REQUIRE(std::get<component1>(conjure_enum<component1>::back()) == std::get<component1>(*conjure_enum<component1>::crbegin()));
}

//-----------------------------------------------------------------------------------------
TEST_CASE("string_to_enum")
{
	REQUIRE(static_cast<int>(conjure_enum<component>::string_to_enum("component::path").value()) == 12);
	REQUIRE(static_cast<int>(conjure_enum<component1>::string_to_enum("path").value()) == 12);
	REQUIRE(static_cast<int>(conjure_enum<component>::string_to_enum("wrong").value_or(component(100))) == 100);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("unscoped_string_to_enum")
{
	REQUIRE(static_cast<int>(conjure_enum<component>::unscoped_string_to_enum("userinfo").value()) == 2);
	REQUIRE(static_cast<int>(conjure_enum<component1>::unscoped_string_to_enum("userinfo").value()) == 2);
	REQUIRE(static_cast<int>(conjure_enum<component>::unscoped_string_to_enum("wrong").value_or(component(100))) == 100);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("int_to_enum")
{
	REQUIRE(conjure_enum<component>::int_to_enum(4).value() == component::password);
	REQUIRE(conjure_enum<component1>::int_to_enum(4).value() == password);
	REQUIRE(conjure_enum<component>::int_to_enum(11).value_or(static_cast<component>(100)) == static_cast<component>(100));
	REQUIRE(conjure_enum<component1>::int_to_enum(11).value_or(static_cast<component1>(100)) == static_cast<component1>(100));
}

//-----------------------------------------------------------------------------------------
TEST_CASE("enum_to_int")
{
	REQUIRE(conjure_enum<component>::enum_to_int(component::password) == 4);
	REQUIRE(conjure_enum<component1>::enum_to_int(password) == 4);
	REQUIRE(conjure_enum<component>::enum_to_underlying(component::password) == 4);
	REQUIRE(conjure_enum<component1>::enum_to_underlying(password) == 4);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("ostream<<")
{
	using ostream_enum_operator::operator<<;
	std::ostringstream ostr;
	ostr << component::host;
	REQUIRE(ostr.str() == "component::host");
	ostr.str("");
	ostr << component1::host;
	REQUIRE(ostr.str() == "host");
	ostr.str("");
	ostr << static_cast<component>(100);
	REQUIRE(ostr.str() == "100");
}

//-----------------------------------------------------------------------------------------
TEST_CASE("get_name")
{
	REQUIRE(conjure_enum<component>::enum_to_string<component::scheme>() == "component::scheme");
	REQUIRE(conjure_enum<component1>::enum_to_string<scheme>() == "scheme");
}

//-----------------------------------------------------------------------------------------
TEST_CASE("type_name")
{
	REQUIRE(conjure_enum<component>::type_name() == "component");
	REQUIRE(conjure_enum<component1>::type_name() == "component1");
	REQUIRE(conjure_enum<numbers>::type_name() == "numbers");
}

//-----------------------------------------------------------------------------------------
TEST_CASE("conjure_type")
{
	REQUIRE(std::string_view(conjure_type<component>::name) == "component");
	REQUIRE(std::string_view(conjure_type<component1>::name) == "component1");
	REQUIRE(std::string_view(conjure_type<numbers>::name) == "numbers");
	REQUIRE(conjure_type<component>::as_string_view() == "component"sv);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("for_each")
{
	int total{};
	auto myfunc { conjure_enum<component>::for_each([](component val, int& tot)
	{
		tot += static_cast<int>(val);
	}, std::ref(total)) };
	myfunc(component::fragment);
	REQUIRE(total == 74);

	struct foo
	{
		void process(component val, int offset, int& tot)
		{
			tot += offset + static_cast<int>(val);
		}
	};
	foo bar;
	total = 0;
	conjure_enum<component>::for_each(&foo::process, &bar, 10, std::ref(total));
	REQUIRE(total == 160);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("for_each_n")
{
	int total{}, count{};
	conjure_enum<component>::for_each_n(3, [](component val, int& tot, int& cnt)
	{
		tot += static_cast<int>(val);
		++cnt;
	}, std::ref(total), std::ref(count));
	REQUIRE(total == 3);
	REQUIRE(count == 3);

	struct foo
	{
		void process(component val, int offset, int& tot)
		{
			tot += offset + static_cast<int>(val);
		}
	};
	foo bar;
	total = 0;
	conjure_enum<component>::for_each_n(3, &foo::process, &bar, 10, std::ref(total));
	REQUIRE(total == 33);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("enum_bitset")
{
	enum_bitset<numbers> eb;
	eb.set<numbers::zero,numbers::two,numbers::five,numbers::nine>();
	REQUIRE(eb.all_of<numbers::zero,numbers::two,numbers::five,numbers::nine>());
	eb.reset<numbers::FIVE>(); // use alias
	REQUIRE(!eb.all_of<numbers::zero,numbers::two,numbers::five,numbers::nine>());
	eb.reset(numbers::nine);
	REQUIRE(!eb.test(numbers::nine));

	enum_bitset<numbers> ec(numbers::one,numbers::three,numbers::six);
	REQUIRE(ec.to_ulong() == (1 << 1 | 1 << 3 | 1 << 6));
	REQUIRE(ec.to_string() == "0001001010"s);
	REQUIRE(ec.to_ulong() == 0b0001001010);
	REQUIRE(ec.to_string('-', '+') == "---+--+-+-"s);
	REQUIRE(enum_bitset<numbers>(0b0101001010).to_string() == "0101001010"s);

	REQUIRE(ec.test<numbers::one>());
	ec.flip<numbers::one>();
	REQUIRE(!ec.test<numbers::one>());
	ec.flip<numbers::one>();
	REQUIRE(ec.test<numbers::one>());
	ec.flip();
	REQUIRE(ec.to_ulong() == 0b1110110101);
	REQUIRE(ec.count() == 7);
	ec.reset<numbers::three>();
	REQUIRE(!ec.test<numbers::three>());
	ec.set<numbers::three>();
	REQUIRE(ec.test<numbers::three>());
	ec.reset(numbers::three);
	REQUIRE(!ec.test<numbers::three>());
	ec.set(numbers::three);
	REQUIRE(ec.test<numbers::three>());
	ec.set(numbers::three, false);
	REQUIRE(ec.test<numbers::three>() == false);
	REQUIRE(ec.any());
}

//-----------------------------------------------------------------------------------------
TEST_CASE("enum_bitset ops")
{
	enum_bitset<numbers> ed(numbers::two,numbers::three,numbers::four,numbers::seven);
	REQUIRE((ed << 1).to_ulong() == 0b0100111000);
	ed <<= 1;
	REQUIRE(ed.to_ulong() == 0b0100111000);
	REQUIRE((ed >> 1).to_ulong() == 0b0010011100);
	ed >>= 1;
	REQUIRE(ed.to_ulong() == 0b0010011100);

	REQUIRE((ed | numbers::one).to_ulong() == 0b0010011110);
	REQUIRE((ed & numbers::two).to_ulong() == 0b100);
	ed |= numbers::one;
	REQUIRE(ed.to_ulong() == 0b0010011110);
	ed &= numbers::one;
	REQUIRE(ed.to_ulong() == 0b10);
	//std::cerr << ed << '\n';

	ed.reset();
	REQUIRE(!ed);
	REQUIRE((ed ^ numbers::one).to_ulong()  == 0b010);
	ed ^= numbers::one;
	REQUIRE(ed.to_ulong() == 0b010);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("enum_bitset ext ops")
{
	enum_bitset<numbers> ed;
	REQUIRE(ed.none());
	ed.set();
	REQUIRE(ed.all());
	enum_bitset<numbers> ee(numbers::one,numbers::two,numbers::three,numbers::four,numbers::five);
	REQUIRE(ee.all_of<numbers::one,numbers::two,numbers::three,numbers::four,numbers::five>());
	REQUIRE(ee.any_of<numbers::two,numbers::three,numbers::five>());
	int a{static_cast<int>(numbers::two)}, b{static_cast<int>(numbers::three)}, c{static_cast<int>(numbers::four)}, d{static_cast<int>(numbers::five)};
	REQUIRE(ee.any_of(a,b,c,d));
	ee.reset<numbers::one,numbers::three,numbers::five>();
	REQUIRE(ee.none_of<numbers::one,numbers::three,numbers::five>());
	REQUIRE(ee.all_of<numbers::two,numbers::four>());
	REQUIRE(ee.not_count() == 10 - 2);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("enum_bitset(std::string_view)")
{
	REQUIRE_THROWS_MATCHES(enum_bitset<numbers>("zero,twenty,two,three", true, ',', false),
		std::invalid_argument, Catch::Matchers::Message("twenty"));
	enum_bitset<numbers> sc("zero,two,three", true, ',');
	REQUIRE(sc.to_ulong() == 0b1101);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("enum_bitset::for_each")
{
	enum_bitset<numbers> ee(0b10101010);
	std::ostringstream ostr;
	ee.for_each([&ostr](numbers val) noexcept
	{
		ostr << conjure_enum<numbers>::enum_to_string(val) << '(' << static_cast<int>(val) << ')' << '\n';
	});
	REQUIRE(ostr.str() ==
R"(numbers::one(1)
numbers::three(3)
numbers::five(5)
numbers::seven(7)
)");

	int total{};
	enum_bitset<numbers> enc(numbers::two,numbers::three,numbers::four,numbers::seven);
	enc.for_each([](numbers val, int& tot)
	{
		tot += static_cast<int>(val);
	}, std::ref(total));
	REQUIRE(total == 16);
}

//-----------------------------------------------------------------------------------------
TEST_CASE("enum_bitset::for_each_n")
{
	enum_bitset<numbers> ee(0b10101010);
	std::ostringstream ostr;
	ee.for_each_n(3, [&ostr](numbers val) noexcept
	{
		ostr << conjure_enum<numbers>::enum_to_string(val) << '(' << static_cast<int>(val) << ')' << '\n';
	});
	REQUIRE(ostr.str() ==
R"(numbers::one(1)
numbers::three(3)
numbers::five(5)
)");

	int total{};
	enum_bitset<numbers> enc(numbers::two,numbers::three,numbers::four,numbers::seven);
	enc.for_each_n(3, [](numbers val, int& tot)
	{
		tot += static_cast<int>(val);
	}, std::ref(total));
	REQUIRE(total == 9);
}

