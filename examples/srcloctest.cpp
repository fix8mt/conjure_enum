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
#include <array>
#include <vector>
#include <cctype>
#include <tuple>
#include <string_view>
#include <source_location>

//-----------------------------------------------------------------------------------------
enum class Namespace_Enum_Type : int { Value };
enum Namespace_Enum_Type1 : int { Value };
struct Foo{};

namespace
{
	enum class Anon_Enum_Type : int { Value };
	enum Anon_Enum_Type1 : int { Value };
	class Anon_Foo{};
}

namespace Namespace
{
	enum class Namespace_Enum_Type : int { Value };
	enum Namespace_Enum_Type1 : int { Value };
	class Namespace_Foo{};
}

template<typename T>
class conjure_enum
{
public:
	static consteval const char *tpeek() noexcept { return std::source_location::current().function_name(); }

	template<T e>
	static consteval const char *epeek() noexcept { return std::source_location::current().function_name(); }
};

template<typename T>
class conjure_type
{
public:
	static consteval const char *tpeek() noexcept { return std::source_location::current().function_name(); }
};

using UType = std::vector<std::tuple<int, char, std::string_view>>;

namespace
{
	enum class NineEnums : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
	enum NineEnums1 : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
	namespace TEST1
	{
		enum class NineEnums : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
		enum NineEnums1 : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
	}
}

namespace TEST
{
	enum class NineEnums : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
	enum NineEnums1 : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
	namespace TEST1
	{
		enum class NineEnums : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
		enum NineEnums1 : int { One, Two, Three, Four, Five, Six, Seven, Eight, Nine };
	}
}

//-----------------------------------------------------------------------------------------
// pass -m to generate markdown version
//-----------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	static constexpr std::array srclocstrs
	{
		"1. scoped enum",
			conjure_enum<Namespace_Enum_Type>::tpeek(),
			conjure_enum<Namespace_Enum_Type>::epeek<Namespace_Enum_Type::Value>(),
			conjure_enum<Namespace_Enum_Type>::epeek<(Namespace_Enum_Type)100>(), "",
		"2. unscoped enum",
			conjure_enum<Namespace_Enum_Type1>::tpeek(),
			conjure_enum<Namespace_Enum_Type1>::epeek<Namespace_Enum_Type1::Value>(),
			conjure_enum<Namespace_Enum_Type1>::epeek<(Namespace_Enum_Type1)100>(), "",
		"3. scoped enum in anonymous namespace",
			conjure_enum<Anon_Enum_Type>::tpeek(),
			conjure_enum<Anon_Enum_Type>::epeek<Anon_Enum_Type::Value>(),
			conjure_enum<Anon_Enum_Type>::epeek<(Anon_Enum_Type)100>(), "",
		"4. unscoped enum in anonymous namespace",
			conjure_enum<Anon_Enum_Type1>::tpeek(),
			conjure_enum<Anon_Enum_Type1>::epeek<Anon_Enum_Type1::Value>(),
			conjure_enum<Anon_Enum_Type1>::epeek<(Anon_Enum_Type1)100>(), "",
		"5. scoped enum in namespace",
			conjure_enum<Namespace::Namespace_Enum_Type>::tpeek(),
			conjure_enum<Namespace::Namespace_Enum_Type>::epeek<Namespace::Namespace_Enum_Type::Value>(),
			conjure_enum<Namespace::Namespace_Enum_Type>::epeek<(Namespace::Namespace_Enum_Type)100>(), "",
		"6. unscoped enum in namespace",
			conjure_enum<Namespace::Namespace_Enum_Type1>::tpeek(),
			conjure_enum<Namespace::Namespace_Enum_Type1>::epeek<Namespace::Namespace_Enum_Type1::Value>(),
			conjure_enum<Namespace::Namespace_Enum_Type1>::epeek<(Namespace::Namespace_Enum_Type1)100>(), "",
		"7. types in named and anonymous namespaces",
			conjure_type<Foo>::tpeek(),
			conjure_type<Namespace::Namespace_Foo>::tpeek(),
			conjure_type<Anon_Foo>::tpeek(), "",
		"8. other types",
			conjure_type<int>::tpeek(),
			conjure_type<std::string_view>::tpeek(),
			conjure_type<UType>::tpeek(),
   };

	bool md{}, ext{}, comp{true}, hlp{};
	auto opts { std::to_array<std::tuple<std::string_view, bool&>> ({{"-m",md},{"-c",comp},{"-e",ext},{"-h",hlp}}) };
	for (int ii{1}; ii < argc; ++ii)
		for (const auto& pp : opts)
			if (std::string_view(argv[ii]) == std::get<0>(pp))
				std::get<1>(pp) ^= 1;

	if (hlp)
	{
		std::cout << "Usage: " << argv[0] << " [-ecmh]" << R"(
-e run extended enum test
-c show compiler (default true)
-m output using markdown
-h help
)";
		return 0;
	}

#define COMBINE1(X,Y) X##Y  // helper macro
#define COMBINE(X,Y) COMBINE1(X,Y)

	if (comp)
	{
		if (md)
			std::cout << "# ";
		std::cout << "Compiler: "
#if defined __clang__
			"Clang: " __VERSION__
#elif defined __GNUC__
			"GCC: " __VERSION__
#elif defined _MSC_VER
			COMBINE("MSVC: ",_MSC_VER)
#else
# error "Not Supported"
#endif
			"\n";
	}

	for (const auto *pp : srclocstrs)
	{
		if (md && std::isdigit(pp[0]))
		{
			if (pp != srclocstrs[0])
				std::cout << "```\n";
			std::cout << "## ";
		}
		if (!md || pp[0])
			std::cout << pp << '\n';
		if (md && std::isdigit(pp[0]))
			std::cout << "```c++\n";
	}
	if (md)
		std::cout << "```\n";

	if (ext)
	{
		std::cout << "\n9. edge enum types\n"
			<< conjure_type<NineEnums>::tpeek() << '\n'
			<< conjure_type<NineEnums1>::tpeek() << '\n'
			<< conjure_type<TEST::NineEnums>::tpeek() << '\n'
			<< conjure_type<TEST::NineEnums1>::tpeek() << '\n';

		std::cout << conjure_type<TEST1::NineEnums>::tpeek() << '\n'
			<< conjure_type<TEST1::NineEnums1>::tpeek() << '\n'
			<< conjure_type<TEST::TEST1::NineEnums>::tpeek() << '\n'
			<< conjure_type<TEST::TEST1::NineEnums1>::tpeek() << '\n';
	}

	return 0;
}
