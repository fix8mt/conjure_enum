//-----------------------------------------------------------------------------------------
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2024 Fix8 Market Technologies Pty Ltd
// SPDX-FileType: SOURCE

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
// Rudimentary conjure_enum and conjure_type implementations that only report the
// output of std::source_location.
// Should compile with any compiler supporting C++20
//----------------------------------------------------------------------------------------
#include <iostream>
#include <array>
#include <map>
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
class conjure_type
{
public:
	static consteval const char *tpeek() noexcept { return std::source_location::current().function_name(); }
};

template<typename T>
class conjure_enum : public conjure_type<T>
{
public:
	template<T e>
	static consteval const char *epeek() noexcept { return std::source_location::current().function_name(); }
};

using UType = std::vector<std::tuple<int, char, std::string_view>>;

#define ENUMS One, Two, Three, Four, Five, Six, Seven, Eight, Nine

namespace
{
	enum class NineEnums : int { ENUMS };
	enum NineEnums1 : int { ENUMS };
	namespace TEST1
	{
		enum class NineEnums : int { ENUMS };
		enum NineEnums1 : int { ENUMS };
	}
}

namespace TEST
{
	enum class NineEnums : int { ENUMS };
	enum NineEnums1 : int { ENUMS };
	namespace TEST1
	{
		enum class NineEnums : int { ENUMS };
		enum NineEnums1 : int { ENUMS };
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
			conjure_type<UType>::tpeek(), "",
		"9. edge enum types",
			conjure_type<NineEnums>::tpeek(),
			conjure_type<NineEnums1>::tpeek(),
			conjure_type<TEST::NineEnums>::tpeek(),
			conjure_type<TEST::NineEnums1>::tpeek(),
			conjure_type<TEST1::NineEnums>::tpeek(),
			conjure_type<TEST1::NineEnums1>::tpeek(),
			conjure_type<TEST::TEST1::NineEnums>::tpeek(),
			conjure_type<TEST::TEST1::NineEnums1>::tpeek()
   };

	bool mkd{}, cpl{true}, hlp{};
	const std::map<std::string_view, bool&> opts { {"-m",mkd},{"-c",cpl},{"-h",hlp} };
	for (const std::vector<std::string_view> args{argv + 1, argv + argc}; const auto pp : args)
		if (auto result{opts.find(pp)}; result != opts.cend())
			result->second ^= true;
	if (hlp)
	{
		std::cout << R"(
This program shows the output generated by your compiler. There are nine different
sets of tests - conjure_enum and conjure_type use this information to inform the
parsing algorithm to extract enum and type info. For most compilers, there will be
few if any differences between compiler versions - certainly few that impact
conjure_enum - however there have been a couple of changes with earlier releases.
Since the output of std::source_location is entirely implementation dependent, future
changes may occur.

Usage: )" << argv[0] << " [-cmh]" << R"(
  -c show compiler (default true)
  -m output using markdown
  -h help
)";
		return 0;
	}

	if (cpl)
	{
#if defined _MSC_VER
#define STRINGIZE(x) #x
#define STRINGIZE_VALUE(x) STRINGIZE(x)
#endif
		if (mkd)
			std::cout << "---\n# ";
		std::cout << "Compiler: "
#if defined __clang__
			"Clang: " __VERSION__
#elif defined __GNUC__
			"GCC: " __VERSION__
#elif defined _MSC_VER
			"MSVC: " STRINGIZE_VALUE(_MSC_VER)
#else
# error "Not Supported"
#endif
			"\n";
	}

	for (const auto *pp : srclocstrs)
	{
		if (mkd && std::isdigit(pp[0]))
		{
			if (pp != srclocstrs[0])
				std::cout << "```\n";
			std::cout << "## ";
		}
		if (!mkd || pp[0])
			std::cout << pp << '\n';
		if (mkd && std::isdigit(pp[0]))
			std::cout << "```c++\n";
	}
	if (mkd)
		std::cout << "```\n";

	return 0;
}
