//-----------------------------------------------------------------------------------------
// SPDX-License-Identifier: MIT
// conjure_enum (header only)
// Copyright (C) 2024 Fix8 Market Technologies Pty Ltd
//   by David L. Dight
// see https://github.com/fix8mt/conjure_enum
//
// Lightweight header-only C++20 enum reflection
//
//		Based on magic_enum <https://github.com/Neargye/magic_enum>
//		Copyright (c) 2019 - 2024 Daniil Goncharov <neargye@gmail.com>.
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
#if __has_include(<format>)
# include <format>
#endif
#if defined(__clang__) || defined(__GNUC__)
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
enum class numbers1 : int { zero=4, one=3, two=2, three, four, five, six, seven, eight, nine };

//-----------------------------------------------------------------------------------------
template<typename T>
const std::string demangle() noexcept
{
#if defined(__clang__) || defined(__GNUC__)
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

int main(int argc, char *argv[])
{
	int total{};
	auto myfunc { conjure_enum<component>::for_each([](component val, int other, int& tot)
	{
		std::cout << static_cast<int>(val) << ' ' << other << '\n';
		tot += static_cast<int>(val);
	}, 10, std::ref(total)) };
	myfunc(component::fragment);
	std::cout << total << '\n';

	enum_bitset<numbers> eb;
	eb.set_all<numbers::zero,numbers::two,numbers::five,numbers::nine>();
	std::cout << eb << '\n';
	std::cout << std::boolalpha << eb.test_all<numbers::zero,numbers::two,numbers::five,numbers::nine>() << '\n';
	eb.clear_all<numbers::five,numbers::two>();
	std::cout << std::boolalpha << eb.test_all(0, 2, 5, 9) << '\n';
	std::cout << std::boolalpha << eb.test_any(0, 2, 5, 9) << '\n';
	std::cout << std::boolalpha << eb.test_all(numbers::zero,numbers::nine) << '\n';
	std::cout << eb << '\n';
	eb.clear(numbers::nine);
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
	std::cout << std::boolalpha << conjure_enum<component>::enum_contains("component::path"sv) << '\n';
	//std::cout << std::boolalpha << conjure_enum<component>::enum_contains(argv[1]) << '\n';
	for(const auto [a, b] : conjure_enum<component>::enum_entries_sorted)
		std::cout << conjure_enum<component>::remove_scope(b) << ' ' << static_cast<int>(a) << '\n';
	for(const auto [a, b] : conjure_enum<numbers1>::enum_entries)
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
	std::cout << demangle<decltype(conjure_enum<component>::enum_scoped_entries)>() << '\n';
	for(const auto [a, b] : conjure_enum<component>::enum_scoped_entries)
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
	for(const auto [a, b] : conjure_enum<component>::enum_scoped_entries)
		std::cout << std::format("{:9} {}\n", a, b);
	for(const auto [value, str] : conjure_enum<component>::enum_entries) // scoped
		std::cout << std::format("{:<2} {}\n", static_cast<int>(value), str);
	std::cout << std::format("{}\n", conjure_enum<component>::enum_contains("component::path"sv));
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
	//enum_bitset<component> et;
	return 0;
}
