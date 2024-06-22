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
#include <iostream>
#include <iomanip>
#include <memory>
#include <map>
#if __has_include(<format>)
# include <format>
#endif
#if defined __clang__  || defined __GNUC__
#include <cxxabi.h>
#endif

#include <fix8/conjure_enum.hpp>

//-----------------------------------------------------------------------------------------
using namespace std::literals::string_view_literals;
using namespace std::literals::string_literals;
using namespace FIX8;

//-----------------------------------------------------------------------------------------
enum class component : int { scheme, authority, userinfo, user, password, host, port, path=12, test=path, query, fragment };
enum component1 : int { scheme, authority, userinfo, user, password, host, port, path=12, query, fragment };
enum class numbers : int { zero, one, two, three, four, five, six, seven, eight, nine };
enum class numbers1 : int { zero1=4, one1=3, two1=2, three1, four1, five1, six1, seven1, eight1, nine1 };

//-----------------------------------------------------------------------------------------
template<typename T>
const std::string demangle() noexcept
{
#if defined __clang__  || defined __GNUC__
	int status;
	std::unique_ptr<char, decltype(&free)> eptr(abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status), &free);
	switch (status)
	{
	case 0: return eptr.get();
	case -1: return "memory allocation failiure"s;
	case -2: return "invalid mangled name"s;
	default: break; // includes -3 invalid argument
	}
	return "invalid argument"s;
#endif
	return typeid(T).name();
}

int main(void)
{
	conjure_enum<component>::for_each_n(3, [](component val, int other) { std::cout << static_cast<int>(val) << ' ' << other << '\n'; }, 200);
	int total{};
	auto myfunc { conjure_enum<component>::for_each([](component val, int other, int& tot)
	{
		std::cout << static_cast<int>(val) << ' ' << other << '\n';
		tot += static_cast<int>(val);
	}, 10, std::ref(total)) };
	myfunc(component::fragment);
	std::cout << total << '\n';

	enum_bitset<numbers> eb;
	eb.set<numbers::zero,numbers::two,numbers::five,numbers::nine>();
	std::cout << eb << '\n';
	std::cout << std::boolalpha << eb.all_of<numbers::zero,numbers::two,numbers::five,numbers::nine>() << '\n';
	eb.reset<numbers::five,numbers::two>();
	std::cout << std::boolalpha << eb.all_of(0, 2, 5, 9) << '\n';
	std::cout << std::boolalpha << eb.any_of(0, 2, 5, 9) << '\n';
	std::cout << std::boolalpha << eb.all_of(numbers::zero,numbers::nine) << '\n';
	std::cout << eb << '\n';
	eb.reset(numbers::nine);
	std::cout << eb << '\n';
	enum_bitset<numbers> ec(numbers::one,numbers::three,numbers::six);
	std::cout << ec << '\n';
	std::cout << ec.to_string('-', '+') << '\n'<< '\n';
	std::cout << ec << '\n';
	ec.flip<numbers::one>();
	std::cout << ec << '\n';
	ec.flip<numbers::one>();
	std::cout << ec << '\n';
	ec.flip();
	std::cout << ec << '\n';
	ec.flip();
	std::cout << ec << '\n';
	std::cout << std::boolalpha << conjure_enum<component>::contains("component::path"sv) << '\n';
	//std::cout << std::boolalpha << conjure_enum<component>::contains(argv[1]) << '\n';
	for(const auto& [a, b] : conjure_enum<component>::sorted_entries)
		std::cout << conjure_enum<component>::remove_scope(b) << ' ' << static_cast<int>(a) << '\n';
		//std::cout << conjure_enum<component>::remove_scope(b) << ' ' << static_cast<int>(a) << '\n';
	for(const auto& [a, b] : conjure_enum<numbers1>::entries)
		std::cout << b << ' ' << static_cast<int>(a) << '\n';
	std::cout << conjure_enum<component>::add_scope("path"sv) << '\n';
	std::cout << conjure_enum<component>::add_scope("component::path"sv) << '\n';
	std::cout << conjure_enum<component1>::add_scope("path"sv) << '\n';
	std::cout << conjure_enum<component>::enum_to_string<component::scheme>() << '\n';
	std::cout << conjure_enum<component1>::enum_to_string<scheme>() << '\n';
	using cje = conjure_enum<component>;
	std::cout << std::boolalpha << cje::has_scope("component::scheme") << '\n';
	std::cout << std::boolalpha << cje::has_scope("scheme") << '\n';
	std::cout << std::boolalpha << cje::has_scope("scheme") << '\n';
	std::cout << conjure_enum<component>::epeek<component::path>() << '\n';
	std::cout << conjure_enum<component>::tpeek() << '\n';
	std::cout << conjure_enum<component>::epeek<static_cast<component>(100)>() << '\n';
	std::cout << demangle<decltype(conjure_enum<component>::scoped_entries)>() << '\n';
	for(const auto& [a, b] : conjure_enum<component>::scoped_entries)
		std::cout << a << ' ' << b << '\n';
	std::cout << "*******************************\n";
#if __has_include(<format>)
	std::cout << std::format(R"("{}")", conjure_enum<component>::enum_to_string(static_cast<component>(100))) << '\n';
	std::cout << std::format(R"("{}")", conjure_enum<component>::enum_to_string(static_cast<component>(100))) << '\n';
#endif
	enum_bitset<numbers> a(0, 1, 2, 3);
	std::cout << a << '\n';
	enum_bitset<numbers> b(numbers::zero, numbers::one, numbers::two, numbers::three);
	std::cout << b << '\n';
	enum_bitset<numbers> c(15);
	std::cout << c << '\n';
#if __has_include(<format>)
	for(const auto& [a2, b2] : conjure_enum<component>::scoped_entries)
		std::cout << std::format("{:9} {}\n", a2, b2);
	for(const auto& [value, str] : conjure_enum<component>::entries) // scoped
		std::cout << std::format("{:<2} {}\n", static_cast<int>(value), str);
	for(const auto& [value, str] : conjure_enum<component>::sorted_entries) // scoped
		std::cout << std::format("{:<2} {}\n", static_cast<int>(value), str);
	std::cout << std::format("{}\n", conjure_enum<component>::contains("component::path"sv));
	std::cout << std::format("\"{}\"\n", conjure_enum<component>::enum_to_string<component::scheme>());
#endif
	auto printer([](numbers val)
	{
		std::cout << conjure_enum<numbers>::enum_to_string(val) << '\n';
	});
	enum_bitset<numbers> ec1("numbers::zero |numbers::two      |numbers::five| numbers::nine", false);
	std::cout << ec1 << '\n';
	ec1.for_each(printer);
	enum_bitset<numbers> ed("zero|two|five|\tnine", true);
	std::cout << ed << '\n';
	ed.for_each(printer);
	try
	{
		enum_bitset<numbers> ee("twenty,two,rubbish,nine", true, ',', false);
		std::cout << ee << '\n';
		ee.for_each(printer);
	}
	catch(const std::invalid_argument& e)
	{
		std::cerr << "exception: " << e.what() << '\n';
	}
	enum_bitset<numbers> ef("one|three|four|eight"sv, true);
	std::cout << ef << '\n';
	ef.for_each(printer);
	int a1{1}, b1{2}, c1{3}, d1{};
	enum_bitset<numbers> eg(d1, a1, b1, c1);
	std::cout << eg << '\n';
	enum_bitset<numbers> ek(0, 1, 2, 3);
	std::cout << ek << '\n';
	std::cout << (ek & 0b111) << '\n';
	ek ^= numbers::two;
	std::cout << ek << '\n';
	enum_bitset<numbers> eh("one 		three four eight		"sv, true, ' ');
	std::cout << eh << '\n';
	using en = conjure_enum<numbers>;
	for (auto itr{en::cbegin()}; itr != en::cend(); ++itr)
		std::cout << static_cast<int>(std::get<0>(*itr)) << ' ' << std::get<1>(*itr) << '\n';
	std::cout << static_cast<int>(std::get<0>(en::front())) << ' ' << std::get<1>(en::front()) << '\n';
	std::cout << static_cast<int>(std::get<0>(en::back())) << ' ' << std::get<1>(en::back()) << '\n';

	struct foo
	{
		void printer(numbers val, int other) const
		{
			std::cout << conjure_enum<numbers>::enum_to_string(val) << ' ' << other << '\n';
		}
	};
	const foo bar;
	conjure_enum<numbers>::for_each_n(3, &foo::printer, &bar, 1000);
	ek.for_each(std::bind(&foo::printer, &bar, std::placeholders::_1, 10));
	ek.for_each(&foo::printer, &bar, 10);
	[[maybe_unused]] enum_bitset<numbers> er("one|three|four|eight"sv, true);

	for (const auto& pp : iterator_adaptor<numbers>())
		std::cout << static_cast<int>(std::get<0>(pp)) << '\n';

	std::cout << '\'' << conjure_type<numbers>::name << '\'' << '\n';
	std::cout << '\'' << conjure_type<conjure_enum<numbers>>::name << '\'' << '\n';

	using test = std::map<std::size_t, std::string_view>;
	using test1 = std::map<std::size_t, foo>;
	std::cout << '\'' << conjure_type<test>::name << '\'' << '\n';
	std::cout << '\'' << conjure_type<test1>::name << '\'' << '\n';
	std::cout << '\'' << conjure_type<conjure_type<conjure_enum<numbers>>>::name << '\'' << '\n';

	auto strv { conjure_type<test>::name };
	std::cout << conjure_type<decltype(strv)>::name << '\n';
	std::cout << conjure_type<std::underlying_type_t<numbers>>::name << '\n';

	for(const auto& [a3, b3] : conjure_enum<component>::unscoped_entries)
		std::cout << static_cast<int>(a3) << ' ' << b3 << '\n';

	std::cout << conjure_type<conjure_type<conjure_enum<numbers>>>::name << '\n';

	using ostream_enum_operator::operator<<;
	std::cout << '"' << component::host << '"' << '\n';
	std::cout << '"' << component1::host << '"' << '\n';
	std::cout << '"' << static_cast<component>(100) << '"' << '\n';
	return 0;
}
