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
#include <tuple>
#include <string_view>
#include <source_location>

//-----------------------------------------------------------------------------------------
enum class Type : int { Value };
enum Type1 : int { Value };

namespace
{
	enum class Anon_Type : int { Value };
	enum Anon_Type1 : int { Value };
}

namespace Namespace
{
	enum class Type : int { Value };
	enum Type1 : int { Value };
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

using foo = std::vector<std::tuple<int, char, std::string_view>>;

//-----------------------------------------------------------------------------------------
int main(void)
{
	static constexpr std::array srclocstrs
	{
		"1. scoped enum",
			conjure_enum<Type>::tpeek(),
			conjure_enum<Type>::epeek<Type::Value>(),
			conjure_enum<Type>::epeek<(Type)100>(), "",
		"2. unscoped enum",
			conjure_enum<Type1>::tpeek(),
			conjure_enum<Type1>::epeek<Type1::Value>(),
			conjure_enum<Type1>::epeek<(Type1)100>(), "",
		"3. scoped enum in anonymous namespace",
			conjure_enum<Anon_Type>::tpeek(),
			conjure_enum<Anon_Type>::epeek<Anon_Type::Value>(),
			conjure_enum<Anon_Type>::epeek<(Anon_Type)100>(), "",
		"4. unscoped enum in anonymous namespace",
			conjure_enum<Anon_Type1>::tpeek(),
			conjure_enum<Anon_Type1>::epeek<Anon_Type1::Value>(),
			conjure_enum<Anon_Type1>::epeek<(Anon_Type1)100>(), "",
		"5. scoped enum in namespace",
			conjure_enum<Namespace::Type>::tpeek(),
			conjure_enum<Namespace::Type>::epeek<Namespace::Type::Value>(),
			conjure_enum<Namespace::Type>::epeek<(Namespace::Type)100>(), "",
		"6. unscoped enum in namespace",
			conjure_enum<Namespace::Type1>::tpeek(),
			conjure_enum<Namespace::Type1>::epeek<Namespace::Type1::Value>(),
			conjure_enum<Namespace::Type1>::epeek<(Namespace::Type1)100>(), "",
		"7. other type",
			conjure_type<int>::tpeek(),
			conjure_type<std::string_view>::tpeek(),
			conjure_type<foo>::tpeek(),
   };
	std::cout << "Compiler: "
#if defined __clang__
		"Clang" ": " __VERSION__
#elif defined __GNUC__
		"GCC" ": " __VERSION__
#elif defined _MSC_VER
		"MSVC" ": " << _MSC_VER <<
#else
# error "conjure_enum not supported by your compiler"
#endif
		"\n";
	for (const auto& pp : srclocstrs)
		std::cout << pp << '\n';
	return 0;
}
