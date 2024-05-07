<!-----------------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------------->
# What is this?

This page shows the output of the included program `srcloctest` from different compilers.
There are seven different sets of tests for each compiler. `conjure_enum` and `conjure_type`
use this information to inform the parsing algorithm that extracts the enum and type info.

```C++
# examples/statictest.cpp
```

---
# Source enums and types
```C++
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
```
---

# Compiler: Clang: Ubuntu Clang 16.0.6 (23ubuntu4)
## 1. scoped enum
```C++
static const char *conjure_enum<Type>::tpeek() [T = Type]
static const char *conjure_enum<Type>::epeek() [T = Type, e = Type::Value]
static const char *conjure_enum<Type>::epeek() [T = Type, e = (Type)100]
```

## 2. unscoped enum
```C++
static const char *conjure_enum<Type1>::tpeek() [T = Type1]
static const char *conjure_enum<Type1>::epeek() [T = Type1, e = Value]
static const char *conjure_enum<Type1>::epeek() [T = Type1, e = (Type1)100]
```

## 3. scoped enum in anonymous namespace
```C++
static const char *conjure_enum<(anonymous namespace)::Anon_Type>::tpeek() [T = (anonymous namespace)::Anon_Type]
static const char *conjure_enum<(anonymous namespace)::Anon_Type>::epeek() [T = (anonymous namespace)::Anon_Type, e = (anonymous namespace)::Anon_Type::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Type>::epeek() [T = (anonymous namespace)::Anon_Type, e = ((anonymous namespace)::Anon_Type)100]
```

## 4. unscoped enum in anonymous namespace
```C++
static const char *conjure_enum<(anonymous namespace)::Anon_Type1>::tpeek() [T = (anonymous namespace)::Anon_Type1]
static const char *conjure_enum<(anonymous namespace)::Anon_Type1>::epeek() [T = (anonymous namespace)::Anon_Type1, e = (anonymous namespace)::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Type1>::epeek() [T = (anonymous namespace)::Anon_Type1, e = ((anonymous namespace)::Anon_Type1)100]
```

## 5. scoped enum in namespace
```C++
static const char *conjure_enum<Namespace::Type>::tpeek() [T = Namespace::Type]
static const char *conjure_enum<Namespace::Type>::epeek() [T = Namespace::Type, e = Namespace::Type::Value]
static const char *conjure_enum<Namespace::Type>::epeek() [T = Namespace::Type, e = (Namespace::Type)100]
```

## 6. unscoped enum in namespace
```C++
static const char *conjure_enum<Namespace::Type1>::tpeek() [T = Namespace::Type1]
static const char *conjure_enum<Namespace::Type1>::epeek() [T = Namespace::Type1, e = Namespace::Value]
static const char *conjure_enum<Namespace::Type1>::epeek() [T = Namespace::Type1, e = (Namespace::Type1)100]
```

## 7. other type
```C++
static const char *conjure_type<int>::tpeek() [T = int]
static const char *conjure_type<std::basic_string_view<char>>::tpeek() [T = std::basic_string_view<char>]
static const char *conjure_type<std::vector<std::tuple<int, char, std::basic_string_view<char>>>>::tpeek() [T = std::vector<std::tuple<int, char, std::basic_string_view<char>>>]
```

---
# Compiler: GCC: 14.0.1 20240412 (experimental) [master r14-9935-g67e1433a94f]
## 1. scoped enum
```C++
static consteval const char* conjure_enum<T>::tpeek() [with T = Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = Type::Value; T = Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Type)100; T = Type]
```

## 2. unscoped enum
```C++
static consteval const char* conjure_enum<T>::tpeek() [with T = Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = Value; T = Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Type1)100; T = Type1]
```

## 3. scoped enum in anonymous namespace
```C++
static consteval const char* conjure_enum<T>::tpeek() [with T = {anonymous}::Anon_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = <unnamed>::Anon_Type::Value; T = {anonymous}::Anon_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (<unnamed>::Anon_Type)100; T = {anonymous}::Anon_Type]
```

## 4. unscoped enum in anonymous namespace
```C++
static consteval const char* conjure_enum<T>::tpeek() [with T = {anonymous}::Anon_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = <unnamed>::Value; T = {anonymous}::Anon_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (<unnamed>::Anon_Type1)100; T = {anonymous}::Anon_Type1]
```

## 5. scoped enum in namespace
```C++
static consteval const char* conjure_enum<T>::tpeek() [with T = Namespace::Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace::Type::Value; T = Namespace::Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace::Type)100; T = Namespace::Type]
```

## 6. unscoped enum in namespace
```C++
static consteval const char* conjure_enum<T>::tpeek() [with T = Namespace::Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace::Value; T = Namespace::Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace::Type1)100; T = Namespace::Type1]
```

## 7. other type
```C++
static consteval const char* conjure_type<T>::tpeek() [with T = int]
static consteval const char* conjure_type<T>::tpeek() [with T = std::basic_string_view<char>]
static consteval const char* conjure_type<T>::tpeek() [with T = std::vector<std::tuple<int, char, std::basic_string_view<char, std::char_traits<char> > > >]
```

---
# Compiler: MSVC: 1939
## 1. scoped enum
```C++
const char *__cdecl conjure_enum<enum Type>::tpeek(void) noexcept
const char *__cdecl conjure_enum<enum Type>::epeek<Type::Value>(void) noexcept
const char *__cdecl conjure_enum<enum Type>::epeek<(enum Type)0x64>(void) noexcept
```

## 2. unscoped enum
```C++
const char *__cdecl conjure_enum<enum Type1>::tpeek(void) noexcept
const char *__cdecl conjure_enum<enum Type1>::epeek<Value>(void) noexcept
const char *__cdecl conjure_enum<enum Type1>::epeek<(enum Type1)0x64>(void) noexcept
```

## 3. scoped enum in anonymous namespace
```C++
const char *__cdecl conjure_enum<enum `anonymous namespace'::Anon_Type>::tpeek(void) noexcept
const char *__cdecl conjure_enum<enum `anonymous namespace'::Anon_Type>::epeek<`anonymous-namespace'::Anon_Type::Value>(void) noexcept
const char *__cdecl conjure_enum<enum `anonymous namespace'::Anon_Type>::epeek<(enum `anonymous-namespace'::Anon_Type)0x64>(void) noexcept
```

## 4. unscoped enum in anonymous namespace
```C++
const char *__cdecl conjure_enum<enum `anonymous namespace'::Anon_Type1>::tpeek(void) noexcept
const char *__cdecl conjure_enum<enum `anonymous namespace'::Anon_Type1>::epeek<`anonymous-namespace'::Value>(void) noexcept
const char *__cdecl conjure_enum<enum `anonymous namespace'::Anon_Type1>::epeek<(enum `anonymous-namespace'::Anon_Type1)0x64>(void) noexcept
```

## 5. scoped enum in namespace
```C++
const char *__cdecl conjure_enum<enum Namespace::Type>::tpeek(void) noexcept
const char *__cdecl conjure_enum<enum Namespace::Type>::epeek<Namespace::Type::Value>(void) noexcept
const char *__cdecl conjure_enum<enum Namespace::Type>::epeek<(enum Namespace::Type)0x64>(void) noexcept
```

## 6. unscoped enum in namespace
```C++
const char *__cdecl conjure_enum<enum Namespace::Type1>::tpeek(void) noexcept
const char *__cdecl conjure_enum<enum Namespace::Type1>::epeek<Namespace::Value>(void) noexcept
const char *__cdecl conjure_enum<enum Namespace::Type1>::epeek<(enum Namespace::Type1)0x64>(void) noexcept
```

## 7. other type
```C++
const char *__cdecl conjure_type<int>::tpeek(void) noexcept
const char *__cdecl conjure_type<class std::basic_string_view<char,struct std::char_traits<char> > >::tpeek(void) noexcept
const char *__cdecl conjure_type<class std::vector<class std::tuple<int,char,class std::basic_string_view<char,struct std::char_traits<char> > >,class std::allocator<class std::tuple<int,char,class std::basic_string_view<char,struct std::char_traits<char> > > > > >::tpeek(void) noexcept
```

---
# Compiler: Clang: Apple LLVM 15.0.0 (clang-1500.3.9.4)
## 1. scoped enum
```C++
static const char *conjure_enum<Type>::tpeek() [T = Type]
static const char *conjure_enum<Type>::epeek() [T = Type, e = Type::Value]
static const char *conjure_enum<Type>::epeek() [T = Type, e = (Type)100]
```

## 2. unscoped enum
```C++
static const char *conjure_enum<Type1>::tpeek() [T = Type1]
static const char *conjure_enum<Type1>::epeek() [T = Type1, e = Value]
static const char *conjure_enum<Type1>::epeek() [T = Type1, e = (Type1)100]
```

## 3. scoped enum in anonymous namespace
```C++
static const char *conjure_enum<(anonymous namespace)::Anon_Type>::tpeek() [T = (anonymous namespace)::Anon_Type]
static const char *conjure_enum<(anonymous namespace)::Anon_Type>::epeek() [T = (anonymous namespace)::Anon_Type, e = (anonymous namespace)::Anon_Type::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Type>::epeek() [T = (anonymous namespace)::Anon_Type, e = ((anonymous namespace)::Anon_Type)100]
```

## 4. unscoped enum in anonymous namespace
```C++
static const char *conjure_enum<(anonymous namespace)::Anon_Type1>::tpeek() [T = (anonymous namespace)::Anon_Type1]
static const char *conjure_enum<(anonymous namespace)::Anon_Type1>::epeek() [T = (anonymous namespace)::Anon_Type1, e = (anonymous namespace)::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Type1>::epeek() [T = (anonymous namespace)::Anon_Type1, e = ((anonymous namespace)::Anon_Type1)100]
```

## 5. scoped enum in namespace
```C++
static const char *conjure_enum<Namespace::Type>::tpeek() [T = Namespace::Type]
static const char *conjure_enum<Namespace::Type>::epeek() [T = Namespace::Type, e = Namespace::Type::Value]
static const char *conjure_enum<Namespace::Type>::epeek() [T = Namespace::Type, e = (Namespace::Type)100]
```

## 6. unscoped enum in namespace
```C++
static const char *conjure_enum<Namespace::Type1>::tpeek() [T = Namespace::Type1]
static const char *conjure_enum<Namespace::Type1>::epeek() [T = Namespace::Type1, e = Namespace::Value]
static const char *conjure_enum<Namespace::Type1>::epeek() [T = Namespace::Type1, e = (Namespace::Type1)100]
```

## 7. other type
```C++
static const char *conjure_type<int>::tpeek() [T = int]
static const char *conjure_type<std::string_view>::tpeek() [T = std::string_view]
static const char *conjure_type<std::vector<std::tuple<int, char, std::string_view>>>::tpeek() [T = std::vector<std::tuple<int, char, std::string_view>>]`1
```

---
# Compiler: GCC: 11.4.0
## 1. scoped enum
```C++
static consteval const char* conjure_enum<T>::tpeek() [with T = Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = Type::Value; T = Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Type)100; T = Type]
```

## 2. unscoped enum
```C++
static consteval const char* conjure_enum<T>::tpeek() [with T = Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = Value; T = Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Type1)100; T = Type1]
```

## 3. scoped enum in anonymous namespace
```C++
static consteval const char* conjure_enum<T>::tpeek() [with T = {anonymous}::Anon_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = <unnamed>::Anon_Type::Value; T = {anonymous}::Anon_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (<unnamed>::Anon_Type)100; T = {anonymous}::Anon_Type]
```

## 4. unscoped enum in anonymous namespace
```C++
static consteval const char* conjure_enum<T>::tpeek() [with T = {anonymous}::Anon_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = <unnamed>::Value; T = {anonymous}::Anon_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (<unnamed>::Anon_Type1)100; T = {anonymous}::Anon_Type1]
```

## 5. scoped enum in namespace
```C++
static consteval const char* conjure_enum<T>::tpeek() [with T = Namespace::Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace::Type::Value; T = Namespace::Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace::Type)100; T = Namespace::Type]
```

## 6. unscoped enum in namespace
```C++
static consteval const char* conjure_enum<T>::tpeek() [with T = Namespace::Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace::Value; T = Namespace::Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace::Type1)100; T = Namespace::Type1]
```

## 7. other type
```C++
static consteval const char* conjure_type<T>::tpeek() [with T = int]
static consteval const char* conjure_type<T>::tpeek() [with T = std::basic_string_view<char>]
static consteval const char* conjure_type<T>::tpeek() [with T = std::vector<std::tuple<int, char, std::basic_string_view<char, std::char_traits<char> > > >]
```
