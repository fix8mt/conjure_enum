<!-----------------------------------------------------------------------------------------
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2024 Fix8 Market Technologies Pty Ltd
// SPDX-FileType: DOCUMENTATION
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
//
//-----------------------------------------------------------------------------------------
// This program generates std::source_location from different compilers.
// There are nine different sets of tests for each compiler. `conjure_enum` and `conjure_type`
// use this information to inform the parsing algorithm that extracts the enum and type info.
// For most compilers, there will be few if any differences between compiler versions - certainly few that impact
// conjure_enum - however there have been a couple of changes with earlier releases. Since std::source_location
// is entirely implementation dependent, future changes may occur.
//
// Belore are some of the parsing rules:
//
// clang
// static const char *FIX8::conjure_enum<component>::epeek() [T = component, e = component::path] // valid
// 																								  |<- 				 ->|
// static const char *FIX8::conjure_enum<component>::epeek() [T = component, e = (component)100] // invalid
//																									  |<-             ->|
// static const char *FIX8::conjure_enum<component>::tpeek() [T = component]
//																				  |<- 		 ->|
// gcc
// static consteval const char* FIX8::conjure_enum<T>::epeek() [with T e = component::path; T = component] // valid
//																						     |<- 				 ->|
// static consteval const char* FIX8::conjure_enum<T>::epeek() [with T e = (component)100; T = component] // invalid
//																		 					  |<-             ->|
// static consteval const char* FIX8::conjure_type<T>::tpeek() [with T = component]
//																							|<- 		  ->|
// msvc
// const char *__cdecl FIX8::conjure_enum<enum numbers>::epeek<numbers::two>(void) noexcept			// valid
//																					|<- 		 ->|
// const char *__cdecl FIX8::conjure_enum<enum numbers>::epeek<(enum numbers)0xa>(void) noexcept	// invalid
//																			|<- 		 		  ->|
// const char *__cdecl FIX8::conjure_enum<enum numbers>::tpeek(void) noexcept
//														|<-    	 ->|
//---------------------------------------------------------------------------------------->
<p align="center">
  <a href="https://www.fix8mt.com"><img src="../assets/conjure_enum_logo.png" width="200"></a>
</p>

# What is this?
This page shows the output of the included program [`srcloctest`](../examples/srcloctest.cpp)
from different compilers.
There are nine different sets of tests for each compiler - `conjure_enum` and `conjure_type`
use this information to inform the parsing algorithm to extract enum and type info.
For most compilers, there will be few if any differences between compiler versions - certainly few that impact
`conjure_enum` - however there have been a couple of changes with earlier releases. Since the output of `std::source_location`
is entirely implementation dependent, future changes may occur.

## Want to add your compiler to this list?
Run `srcloctest` with the `-m` switch, append the output to [this document](./reference/source_location.md) in your fork
and open a pull request to the [dev](https://github.com/fix8mt/conjure_enum/tree/dev) branch.

---
# Compiler: Clang: Ubuntu Clang 16.0.6 (23ubuntu4)
## 1. scoped enum
```c++
static const char *conjure_type<Namespace_Enum_Type>::tpeek() [T = Namespace_Enum_Type]
static const char *conjure_enum<Namespace_Enum_Type>::epeek() [T = Namespace_Enum_Type, e = Namespace_Enum_Type::Value]
static const char *conjure_enum<Namespace_Enum_Type>::epeek() [T = Namespace_Enum_Type, e = (Namespace_Enum_Type)100]
```
## 2. unscoped enum
```c++
static const char *conjure_type<Namespace_Enum_Type1>::tpeek() [T = Namespace_Enum_Type1]
static const char *conjure_enum<Namespace_Enum_Type1>::epeek() [T = Namespace_Enum_Type1, e = Value]
static const char *conjure_enum<Namespace_Enum_Type1>::epeek() [T = Namespace_Enum_Type1, e = (Namespace_Enum_Type1)100]
```
## 3. scoped enum in anonymous namespace
```c++
static const char *conjure_type<(anonymous namespace)::Anon_Enum_Type>::tpeek() [T = (anonymous namespace)::Anon_Enum_Type]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type>::epeek() [T = (anonymous namespace)::Anon_Enum_Type, e = (anonymous namespace)::Anon_Enum_Type::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type>::epeek() [T = (anonymous namespace)::Anon_Enum_Type, e = ((anonymous namespace)::Anon_Enum_Type)100]
```
## 4. unscoped enum in anonymous namespace
```c++
static const char *conjure_type<(anonymous namespace)::Anon_Enum_Type1>::tpeek() [T = (anonymous namespace)::Anon_Enum_Type1]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type1>::epeek() [T = (anonymous namespace)::Anon_Enum_Type1, e = (anonymous namespace)::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type1>::epeek() [T = (anonymous namespace)::Anon_Enum_Type1, e = ((anonymous namespace)::Anon_Enum_Type1)100]
```
## 5. scoped enum in namespace
```c++
static const char *conjure_type<Namespace::Namespace_Enum_Type>::tpeek() [T = Namespace::Namespace_Enum_Type]
static const char *conjure_enum<Namespace::Namespace_Enum_Type>::epeek() [T = Namespace::Namespace_Enum_Type, e = Namespace::Namespace_Enum_Type::Value]
static const char *conjure_enum<Namespace::Namespace_Enum_Type>::epeek() [T = Namespace::Namespace_Enum_Type, e = (Namespace::Namespace_Enum_Type)100]
```
## 6. unscoped enum in namespace
```c++
static const char *conjure_type<Namespace::Namespace_Enum_Type1>::tpeek() [T = Namespace::Namespace_Enum_Type1]
static const char *conjure_enum<Namespace::Namespace_Enum_Type1>::epeek() [T = Namespace::Namespace_Enum_Type1, e = Namespace::Value]
static const char *conjure_enum<Namespace::Namespace_Enum_Type1>::epeek() [T = Namespace::Namespace_Enum_Type1, e = (Namespace::Namespace_Enum_Type1)100]
```
## 7. types in named and anonymous namespaces
```c++
static const char *conjure_type<Foo>::tpeek() [T = Foo]
static const char *conjure_type<Namespace::Namespace_Foo>::tpeek() [T = Namespace::Namespace_Foo]
static const char *conjure_type<(anonymous namespace)::Anon_Foo>::tpeek() [T = (anonymous namespace)::Anon_Foo]
```
## 8. other types
```c++
static const char *conjure_type<int>::tpeek() [T = int]
static const char *conjure_type<std::basic_string_view<char>>::tpeek() [T = std::basic_string_view<char>]
static const char *conjure_type<std::vector<std::tuple<int, char, std::basic_string_view<char>>>>::tpeek() [T = std::vector<std::tuple<int, char, std::basic_string_view<char>>>]
```
## 9. edge enum types
```c++
static const char *conjure_type<(anonymous namespace)::NineEnums>::tpeek() [T = (anonymous namespace)::NineEnums]
static const char *conjure_type<(anonymous namespace)::NineEnums1>::tpeek() [T = (anonymous namespace)::NineEnums1]
static const char *conjure_type<TEST::NineEnums>::tpeek() [T = TEST::NineEnums]
static const char *conjure_type<TEST::NineEnums1>::tpeek() [T = TEST::NineEnums1]
static const char *conjure_type<(anonymous namespace)::TEST1::NineEnums>::tpeek() [T = (anonymous namespace)::TEST1::NineEnums]
static const char *conjure_type<(anonymous namespace)::TEST1::NineEnums1>::tpeek() [T = (anonymous namespace)::TEST1::NineEnums1]
static const char *conjure_type<TEST::TEST1::NineEnums>::tpeek() [T = TEST::TEST1::NineEnums]
static const char *conjure_type<TEST::TEST1::NineEnums1>::tpeek() [T = TEST::TEST1::NineEnums1]
```

---
# Compiler: GCC: 14.0.1 20240412 (experimental) [master r14-9935-g67e1433a94f]
## 1. scoped enum
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace_Enum_Type::Value; T = Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace_Enum_Type)100; T = Namespace_Enum_Type]
```
## 2. unscoped enum
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = Value; T = Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace_Enum_Type1)100; T = Namespace_Enum_Type1]
```
## 3. scoped enum in anonymous namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::Anon_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = <unnamed>::Anon_Enum_Type::Value; T = {anonymous}::Anon_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (<unnamed>::Anon_Enum_Type)100; T = {anonymous}::Anon_Enum_Type]
```
## 4. unscoped enum in anonymous namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::Anon_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = <unnamed>::Value; T = {anonymous}::Anon_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (<unnamed>::Anon_Enum_Type1)100; T = {anonymous}::Anon_Enum_Type1]
```
## 5. scoped enum in namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace::Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace::Namespace_Enum_Type::Value; T = Namespace::Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace::Namespace_Enum_Type)100; T = Namespace::Namespace_Enum_Type]
```
## 6. unscoped enum in namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace::Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace::Value; T = Namespace::Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace::Namespace_Enum_Type1)100; T = Namespace::Namespace_Enum_Type1]
```
## 7. types in named and anonymous namespaces
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Foo]
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace::Namespace_Foo]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::Anon_Foo]
```
## 8. other types
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = int]
static consteval const char* conjure_type<T>::tpeek() [with T = std::basic_string_view<char>]
static consteval const char* conjure_type<T>::tpeek() [with T = std::vector<std::tuple<int, char, std::basic_string_view<char, std::char_traits<char> > > >]
```
## 9. edge enum types
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::NineEnums1]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::NineEnums1]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::TEST1::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::TEST1::NineEnums1]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::TEST1::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::TEST1::NineEnums1]
```

---
# Compiler: MSVC: 1940
## 1. scoped enum
```c++
const char *__cdecl conjure_type<enum Namespace_Enum_Type>::tpeek(void) noexcept
const char *__cdecl conjure_enum<enum Namespace_Enum_Type>::epeek<Namespace_Enum_Type::Value>(void) noexcept
const char *__cdecl conjure_enum<enum Namespace_Enum_Type>::epeek<(enum Namespace_Enum_Type)0x64>(void) noexcept
```
## 2. unscoped enum
```c++
const char *__cdecl conjure_type<enum Namespace_Enum_Type1>::tpeek(void) noexcept
const char *__cdecl conjure_enum<enum Namespace_Enum_Type1>::epeek<Value>(void) noexcept
const char *__cdecl conjure_enum<enum Namespace_Enum_Type1>::epeek<(enum Namespace_Enum_Type1)0x64>(void) noexcept
```
## 3. scoped enum in anonymous namespace
```c++
const char *__cdecl conjure_type<enum `anonymous namespace'::Anon_Enum_Type>::tpeek(void) noexcept
const char *__cdecl conjure_enum<enum `anonymous namespace'::Anon_Enum_Type>::epeek<`anonymous-namespace'::Anon_Enum_Type::Value>(void) noexcept
const char *__cdecl conjure_enum<enum `anonymous namespace'::Anon_Enum_Type>::epeek<(enum `anonymous-namespace'::Anon_Enum_Type)0x64>(void) noexcept
```
## 4. unscoped enum in anonymous namespace
```c++
const char *__cdecl conjure_type<enum `anonymous namespace'::Anon_Enum_Type1>::tpeek(void) noexcept
const char *__cdecl conjure_enum<enum `anonymous namespace'::Anon_Enum_Type1>::epeek<`anonymous-namespace'::Value>(void) noexcept
const char *__cdecl conjure_enum<enum `anonymous namespace'::Anon_Enum_Type1>::epeek<(enum `anonymous-namespace'::Anon_Enum_Type1)0x64>(void) noexcept
```
## 5. scoped enum in namespace
```c++
const char *__cdecl conjure_type<enum Namespace::Namespace_Enum_Type>::tpeek(void) noexcept
const char *__cdecl conjure_enum<enum Namespace::Namespace_Enum_Type>::epeek<Namespace::Namespace_Enum_Type::Value>(void) noexcept
const char *__cdecl conjure_enum<enum Namespace::Namespace_Enum_Type>::epeek<(enum Namespace::Namespace_Enum_Type)0x64>(void) noexcept
```
## 6. unscoped enum in namespace
```c++
const char *__cdecl conjure_type<enum Namespace::Namespace_Enum_Type1>::tpeek(void) noexcept
const char *__cdecl conjure_enum<enum Namespace::Namespace_Enum_Type1>::epeek<Namespace::Value>(void) noexcept
const char *__cdecl conjure_enum<enum Namespace::Namespace_Enum_Type1>::epeek<(enum Namespace::Namespace_Enum_Type1)0x64>(void) noexcept
```
## 7. types in named and anonymous namespaces
```c++
const char *__cdecl conjure_type<struct Foo>::tpeek(void) noexcept
const char *__cdecl conjure_type<class Namespace::Namespace_Foo>::tpeek(void) noexcept
const char *__cdecl conjure_type<class `anonymous namespace'::Anon_Foo>::tpeek(void) noexcept
```
## 8. other types
```c++
const char *__cdecl conjure_type<int>::tpeek(void) noexcept
const char *__cdecl conjure_type<class std::basic_string_view<char,struct std::char_traits<char> > >::tpeek(void) noexcept
const char *__cdecl conjure_type<class std::vector<class std::tuple<int,char,class std::basic_string_view<char,struct std::char_traits<char> > >,class std::allocator<class std::tuple<int,char,class std::basic_string_view<char,struct std::char_traits<char> > > > > >::tpeek(void) noexcept
```
## 9. edge enum types
```c++
const char *__cdecl conjure_type<enum `anonymous namespace'::NineEnums>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum `anonymous namespace'::NineEnums1>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum TEST::NineEnums>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum TEST::NineEnums1>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum `anonymous namespace'::TEST1::NineEnums>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum `anonymous namespace'::TEST1::NineEnums1>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum TEST::TEST1::NineEnums>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum TEST::TEST1::NineEnums1>::tpeek(void) noexcept
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

## 8. other types
```c++
const char *__cdecl conjure_type<int>::tpeek(void) noexcept
const char *__cdecl conjure_type<class std::basic_string_view<char,struct std::char_traits<char> > >::tpeek(void) noexcept
const char *__cdecl conjure_type<class std::vector<class std::tuple<int,char,class std::basic_string_view<char,struct std::char_traits<char> > >,class std::allocator<class std::tuple<int,char,class std::basic_string_view<char,struct std::char_traits<char> > > > > >::tpeek(void) noexcept
```
## 9. edge enum types
```c++
const char *__cdecl conjure_type<enum `anonymous namespace'::NineEnums>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum `anonymous namespace'::NineEnums1>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum TEST::NineEnums>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum TEST::NineEnums1>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum `anonymous namespace'::TEST1::NineEnums>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum `anonymous namespace'::TEST1::NineEnums1>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum TEST::TEST1::NineEnums>::tpeek(void) noexcept
const char *__cdecl conjure_type<enum TEST::TEST1::NineEnums1>::tpeek(void) noexcept
```

---
# Compiler: Clang: Apple LLVM 15.0.0 (clang-1500.3.9.4)
## 1. scoped enum
```c++
static const char *conjure_type<Namespace_Enum_Type>::tpeek() [T = Namespace_Enum_Type]
static const char *conjure_enum<Namespace_Enum_Type>::epeek() [T = Namespace_Enum_Type, e = Namespace_Enum_Type::Value]
static const char *conjure_enum<Namespace_Enum_Type>::epeek() [T = Namespace_Enum_Type, e = (Namespace_Enum_Type)100]
```
## 2. unscoped enum
```c++
static const char *conjure_type<Namespace_Enum_Type1>::tpeek() [T = Namespace_Enum_Type1]
static const char *conjure_enum<Namespace_Enum_Type1>::epeek() [T = Namespace_Enum_Type1, e = Value]
static const char *conjure_enum<Namespace_Enum_Type1>::epeek() [T = Namespace_Enum_Type1, e = (Namespace_Enum_Type1)100]
```
## 3. scoped enum in anonymous namespace
```c++
static const char *conjure_type<(anonymous namespace)::Anon_Enum_Type>::tpeek() [T = (anonymous namespace)::Anon_Enum_Type]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type>::epeek() [T = (anonymous namespace)::Anon_Enum_Type, e = (anonymous namespace)::Anon_Enum_Type::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type>::epeek() [T = (anonymous namespace)::Anon_Enum_Type, e = ((anonymous namespace)::Anon_Enum_Type)100]
```
## 4. unscoped enum in anonymous namespace
```c++
static const char *conjure_type<(anonymous namespace)::Anon_Enum_Type1>::tpeek() [T = (anonymous namespace)::Anon_Enum_Type1]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type1>::epeek() [T = (anonymous namespace)::Anon_Enum_Type1, e = (anonymous namespace)::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type1>::epeek() [T = (anonymous namespace)::Anon_Enum_Type1, e = ((anonymous namespace)::Anon_Enum_Type1)100]
```
## 5. scoped enum in namespace
```c++
static const char *conjure_type<Namespace::Namespace_Enum_Type>::tpeek() [T = Namespace::Namespace_Enum_Type]
static const char *conjure_enum<Namespace::Namespace_Enum_Type>::epeek() [T = Namespace::Namespace_Enum_Type, e = Namespace::Namespace_Enum_Type::Value]
static const char *conjure_enum<Namespace::Namespace_Enum_Type>::epeek() [T = Namespace::Namespace_Enum_Type, e = (Namespace::Namespace_Enum_Type)100]
```
## 6. unscoped enum in namespace
```c++
static const char *conjure_type<Namespace::Namespace_Enum_Type1>::tpeek() [T = Namespace::Namespace_Enum_Type1]
static const char *conjure_enum<Namespace::Namespace_Enum_Type1>::epeek() [T = Namespace::Namespace_Enum_Type1, e = Namespace::Value]
static const char *conjure_enum<Namespace::Namespace_Enum_Type1>::epeek() [T = Namespace::Namespace_Enum_Type1, e = (Namespace::Namespace_Enum_Type1)100]
```
## 7. types in named and anonymous namespaces
```c++
static const char *conjure_type<Foo>::tpeek() [T = Foo]
static const char *conjure_type<Namespace::Namespace_Foo>::tpeek() [T = Namespace::Namespace_Foo]
static const char *conjure_type<(anonymous namespace)::Anon_Foo>::tpeek() [T = (anonymous namespace)::Anon_Foo]
```
## 8. other types
```c++
static const char *conjure_type<int>::tpeek() [T = int]
static const char *conjure_type<std::string_view>::tpeek() [T = std::string_view]
static const char *conjure_type<std::vector<std::tuple<int, char, std::string_view>>>::tpeek() [T = std::vector<std::tuple<int, char, std::string_view>>]
```
## 9. edge enum types
```c++
static const char *conjure_type<(anonymous namespace)::NineEnums>::tpeek() [T = (anonymous namespace)::NineEnums]
static const char *conjure_type<(anonymous namespace)::NineEnums1>::tpeek() [T = (anonymous namespace)::NineEnums1]
static const char *conjure_type<TEST::NineEnums>::tpeek() [T = TEST::NineEnums]
static const char *conjure_type<TEST::NineEnums1>::tpeek() [T = TEST::NineEnums1]
static const char *conjure_type<(anonymous namespace)::TEST1::NineEnums>::tpeek() [T = (anonymous namespace)::TEST1::NineEnums]
static const char *conjure_type<(anonymous namespace)::TEST1::NineEnums1>::tpeek() [T = (anonymous namespace)::TEST1::NineEnums1]
static const char *conjure_type<TEST::TEST1::NineEnums>::tpeek() [T = TEST::TEST1::NineEnums]
static const char *conjure_type<TEST::TEST1::NineEnums1>::tpeek() [T = TEST::TEST1::NineEnums1]
```

---
# Compiler: GCC: 11.4.0
## 1. scoped enum
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace_Enum_Type::Value; T = Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace_Enum_Type)100; T = Namespace_Enum_Type]
```
## 2. unscoped enum
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = Value; T = Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace_Enum_Type1)100; T = Namespace_Enum_Type1]
```
## 3. scoped enum in anonymous namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::Anon_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = <unnamed>::Anon_Enum_Type::Value; T = {anonymous}::Anon_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (<unnamed>::Anon_Enum_Type)100; T = {anonymous}::Anon_Enum_Type]
```
## 4. unscoped enum in anonymous namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::Anon_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = <unnamed>::Value; T = {anonymous}::Anon_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (<unnamed>::Anon_Enum_Type1)100; T = {anonymous}::Anon_Enum_Type1]
```
## 5. scoped enum in namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace::Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace::Namespace_Enum_Type::Value; T = Namespace::Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace::Namespace_Enum_Type)100; T = Namespace::Namespace_Enum_Type]
```
## 6. unscoped enum in namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace::Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace::Value; T = Namespace::Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace::Namespace_Enum_Type1)100; T = Namespace::Namespace_Enum_Type1]
```
## 7. types in named and anonymous namespaces
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Foo]
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace::Namespace_Foo]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::Anon_Foo]
```
## 8. other types
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = int]
static consteval const char* conjure_type<T>::tpeek() [with T = std::basic_string_view<char>]
static consteval const char* conjure_type<T>::tpeek() [with T = std::vector<std::tuple<int, char, std::basic_string_view<char, std::char_traits<char> > > >]
```
## 9. edge enum types
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::NineEnums1]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::NineEnums1]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::TEST1::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::TEST1::NineEnums1]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::TEST1::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::TEST1::NineEnums1]
```

---
# Compiler: Clang: Ubuntu Clang 18.1.3 (1ubuntu1)
## 1. scoped enum
```c++
static const char *conjure_type<Namespace_Enum_Type>::tpeek() [T = Namespace_Enum_Type]
static const char *conjure_enum<Namespace_Enum_Type>::epeek() [T = Namespace_Enum_Type, e = Namespace_Enum_Type::Value]
static const char *conjure_enum<Namespace_Enum_Type>::epeek() [T = Namespace_Enum_Type, e = (Namespace_Enum_Type)100]
```
## 2. unscoped enum
```c++
static const char *conjure_type<Namespace_Enum_Type1>::tpeek() [T = Namespace_Enum_Type1]
static const char *conjure_enum<Namespace_Enum_Type1>::epeek() [T = Namespace_Enum_Type1, e = Value]
static const char *conjure_enum<Namespace_Enum_Type1>::epeek() [T = Namespace_Enum_Type1, e = (Namespace_Enum_Type1)100]
```
## 3. scoped enum in anonymous namespace
```c++
static const char *conjure_type<(anonymous namespace)::Anon_Enum_Type>::tpeek() [T = (anonymous namespace)::Anon_Enum_Type]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type>::epeek() [T = (anonymous namespace)::Anon_Enum_Type, e = (anonymous namespace)::Anon_Enum_Type::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type>::epeek() [T = (anonymous namespace)::Anon_Enum_Type, e = ((anonymous namespace)::Anon_Enum_Type)100]
```
## 4. unscoped enum in anonymous namespace
```c++
static const char *conjure_type<(anonymous namespace)::Anon_Enum_Type1>::tpeek() [T = (anonymous namespace)::Anon_Enum_Type1]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type1>::epeek() [T = (anonymous namespace)::Anon_Enum_Type1, e = (anonymous namespace)::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type1>::epeek() [T = (anonymous namespace)::Anon_Enum_Type1, e = ((anonymous namespace)::Anon_Enum_Type1)100]
```
## 5. scoped enum in namespace
```c++
static const char *conjure_type<Namespace::Namespace_Enum_Type>::tpeek() [T = Namespace::Namespace_Enum_Type]
static const char *conjure_enum<Namespace::Namespace_Enum_Type>::epeek() [T = Namespace::Namespace_Enum_Type, e = Namespace::Namespace_Enum_Type::Value]
static const char *conjure_enum<Namespace::Namespace_Enum_Type>::epeek() [T = Namespace::Namespace_Enum_Type, e = (Namespace::Namespace_Enum_Type)100]
```
## 6. unscoped enum in namespace
```c++
static const char *conjure_type<Namespace::Namespace_Enum_Type1>::tpeek() [T = Namespace::Namespace_Enum_Type1]
static const char *conjure_enum<Namespace::Namespace_Enum_Type1>::epeek() [T = Namespace::Namespace_Enum_Type1, e = Namespace::Value]
static const char *conjure_enum<Namespace::Namespace_Enum_Type1>::epeek() [T = Namespace::Namespace_Enum_Type1, e = (Namespace::Namespace_Enum_Type1)100]
```
## 7. types in named and anonymous namespaces
```c++
static const char *conjure_type<Foo>::tpeek() [T = Foo]
static const char *conjure_type<Namespace::Namespace_Foo>::tpeek() [T = Namespace::Namespace_Foo]
static const char *conjure_type<(anonymous namespace)::Anon_Foo>::tpeek() [T = (anonymous namespace)::Anon_Foo]
```
## 8. other types
```c++
static const char *conjure_type<int>::tpeek() [T = int]
static const char *conjure_type<std::basic_string_view<char>>::tpeek() [T = std::basic_string_view<char>]
static const char *conjure_type<std::vector<std::tuple<int, char, std::basic_string_view<char>>>>::tpeek() [T = std::vector<std::tuple<int, char, std::basic_string_view<char>>>]
```
## 9. edge enum types
```c++
static const char *conjure_type<(anonymous namespace)::NineEnums>::tpeek() [T = (anonymous namespace)::NineEnums]
static const char *conjure_type<(anonymous namespace)::NineEnums1>::tpeek() [T = (anonymous namespace)::NineEnums1]
static const char *conjure_type<TEST::NineEnums>::tpeek() [T = TEST::NineEnums]
static const char *conjure_type<TEST::NineEnums1>::tpeek() [T = TEST::NineEnums1]
static const char *conjure_type<(anonymous namespace)::TEST1::NineEnums>::tpeek() [T = (anonymous namespace)::TEST1::NineEnums]
static const char *conjure_type<(anonymous namespace)::TEST1::NineEnums1>::tpeek() [T = (anonymous namespace)::TEST1::NineEnums1]
static const char *conjure_type<TEST::TEST1::NineEnums>::tpeek() [T = TEST::TEST1::NineEnums]
static const char *conjure_type<TEST::TEST1::NineEnums1>::tpeek() [T = TEST::TEST1::NineEnums1]
```

---
# Compiler: GCC: 12.3.0
## 1. scoped enum
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace_Enum_Type::Value; T = Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace_Enum_Type)100; T = Namespace_Enum_Type]
```
## 2. unscoped enum
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = Value; T = Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace_Enum_Type1)100; T = Namespace_Enum_Type1]
```
## 3. scoped enum in anonymous namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::Anon_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = <unnamed>::Anon_Enum_Type::Value; T = {anonymous}::Anon_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (<unnamed>::Anon_Enum_Type)100; T = {anonymous}::Anon_Enum_Type]
```
## 4. unscoped enum in anonymous namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::Anon_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = <unnamed>::Value; T = {anonymous}::Anon_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (<unnamed>::Anon_Enum_Type1)100; T = {anonymous}::Anon_Enum_Type1]
```
## 5. scoped enum in namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace::Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace::Namespace_Enum_Type::Value; T = Namespace::Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace::Namespace_Enum_Type)100; T = Namespace::Namespace_Enum_Type]
```
## 6. unscoped enum in namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace::Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace::Value; T = Namespace::Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace::Namespace_Enum_Type1)100; T = Namespace::Namespace_Enum_Type1]
```
## 7. types in named and anonymous namespaces
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Foo]
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace::Namespace_Foo]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::Anon_Foo]
```
## 8. other types
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = int]
static consteval const char* conjure_type<T>::tpeek() [with T = std::basic_string_view<char>]
static consteval const char* conjure_type<T>::tpeek() [with T = std::vector<std::tuple<int, char, std::basic_string_view<char, std::char_traits<char> > > >]
```
## 9. edge enum types
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::NineEnums1]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::NineEnums1]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::TEST1::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::TEST1::NineEnums1]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::TEST1::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::TEST1::NineEnums1]
```

# Compiler: GCC: 13.2.0
## 1. scoped enum
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace_Enum_Type::Value; T = Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace_Enum_Type)100; T = Namespace_Enum_Type]
```
## 2. unscoped enum
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = Value; T = Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace_Enum_Type1)100; T = Namespace_Enum_Type1]
```
## 3. scoped enum in anonymous namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::Anon_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = <unnamed>::Anon_Enum_Type::Value; T = {anonymous}::Anon_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (<unnamed>::Anon_Enum_Type)100; T = {anonymous}::Anon_Enum_Type]
```
## 4. unscoped enum in anonymous namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::Anon_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = <unnamed>::Value; T = {anonymous}::Anon_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (<unnamed>::Anon_Enum_Type1)100; T = {anonymous}::Anon_Enum_Type1]
```
## 5. scoped enum in namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace::Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace::Namespace_Enum_Type::Value; T = Namespace::Namespace_Enum_Type]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace::Namespace_Enum_Type)100; T = Namespace::Namespace_Enum_Type]
```
## 6. unscoped enum in namespace
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace::Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = Namespace::Value; T = Namespace::Namespace_Enum_Type1]
static consteval const char* conjure_enum<T>::epeek() [with T e = (Namespace::Namespace_Enum_Type1)100; T = Namespace::Namespace_Enum_Type1]
```
## 7. types in named and anonymous namespaces
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = Foo]
static consteval const char* conjure_type<T>::tpeek() [with T = Namespace::Namespace_Foo]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::Anon_Foo]
```
## 8. other types
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = int]
static consteval const char* conjure_type<T>::tpeek() [with T = std::basic_string_view<char>]
static consteval const char* conjure_type<T>::tpeek() [with T = std::vector<std::tuple<int, char, std::basic_string_view<char, std::char_traits<char> > > >]
```
## 9. edge enum types
```c++
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::NineEnums1]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::NineEnums1]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::TEST1::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = {anonymous}::TEST1::NineEnums1]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::TEST1::NineEnums]
static consteval const char* conjure_type<T>::tpeek() [with T = TEST::TEST1::NineEnums1]
```

# Compiler: Clang: Ubuntu Clang 17.0.6 (9ubuntu1)
## 1. scoped enum
```c++
static const char *conjure_type<Namespace_Enum_Type>::tpeek() [T = Namespace_Enum_Type]
static const char *conjure_enum<Namespace_Enum_Type>::epeek() [T = Namespace_Enum_Type, e = Namespace_Enum_Type::Value]
static const char *conjure_enum<Namespace_Enum_Type>::epeek() [T = Namespace_Enum_Type, e = (Namespace_Enum_Type)100]
```
## 2. unscoped enum
```c++
static const char *conjure_type<Namespace_Enum_Type1>::tpeek() [T = Namespace_Enum_Type1]
static const char *conjure_enum<Namespace_Enum_Type1>::epeek() [T = Namespace_Enum_Type1, e = Value]
static const char *conjure_enum<Namespace_Enum_Type1>::epeek() [T = Namespace_Enum_Type1, e = (Namespace_Enum_Type1)100]
```
## 3. scoped enum in anonymous namespace
```c++
static const char *conjure_type<(anonymous namespace)::Anon_Enum_Type>::tpeek() [T = (anonymous namespace)::Anon_Enum_Type]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type>::epeek() [T = (anonymous namespace)::Anon_Enum_Type, e = (anonymous namespace)::Anon_Enum_Type::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type>::epeek() [T = (anonymous namespace)::Anon_Enum_Type, e = ((anonymous namespace)::Anon_Enum_Type)100]
```
## 4. unscoped enum in anonymous namespace
```c++
static const char *conjure_type<(anonymous namespace)::Anon_Enum_Type1>::tpeek() [T = (anonymous namespace)::Anon_Enum_Type1]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type1>::epeek() [T = (anonymous namespace)::Anon_Enum_Type1, e = (anonymous namespace)::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type1>::epeek() [T = (anonymous namespace)::Anon_Enum_Type1, e = ((anonymous namespace)::Anon_Enum_Type1)100]
```                                                                                                                                                                                                                                                  ## 5. scoped enum in namespace
```c++
static const char *conjure_type<Namespace::Namespace_Enum_Type>::tpeek() [T = Namespace::Namespace_Enum_Type]
static const char *conjure_enum<Namespace::Namespace_Enum_Type>::epeek() [T = Namespace::Namespace_Enum_Type, e = Namespace::Namespace_Enum_Type::Value]
static const char *conjure_enum<Namespace::Namespace_Enum_Type>::epeek() [T = Namespace::Namespace_Enum_Type, e = (Namespace::Namespace_Enum_Type)100]
```
## 6. unscoped enum in namespace
```c++
static const char *conjure_type<Namespace::Namespace_Enum_Type1>::tpeek() [T = Namespace::Namespace_Enum_Type1]
static const char *conjure_enum<Namespace::Namespace_Enum_Type1>::epeek() [T = Namespace::Namespace_Enum_Type1, e = Namespace::Value]
static const char *conjure_enum<Namespace::Namespace_Enum_Type1>::epeek() [T = Namespace::Namespace_Enum_Type1, e = (Namespace::Namespace_Enum_Type1)100]
```
## 7. types in named and anonymous namespaces
```c++
static const char *conjure_type<Foo>::tpeek() [T = Foo]
static const char *conjure_type<Namespace::Namespace_Foo>::tpeek() [T = Namespace::Namespace_Foo]
static const char *conjure_type<(anonymous namespace)::Anon_Foo>::tpeek() [T = (anonymous namespace)::Anon_Foo]
```
## 8. other types
```c++
static const char *conjure_type<int>::tpeek() [T = int]
static const char *conjure_type<std::basic_string_view<char>>::tpeek() [T = std::basic_string_view<char>]
static const char *conjure_type<std::vector<std::tuple<int, char, std::basic_string_view<char>>>>::tpeek() [T = std::vector<std::tuple<int, char, std::basic_string_view<char>>>]
```
## 9. edge enum types
```c++
static const char *conjure_type<(anonymous namespace)::NineEnums>::tpeek() [T = (anonymous namespace)::NineEnums]
static const char *conjure_type<(anonymous namespace)::NineEnums1>::tpeek() [T = (anonymous namespace)::NineEnums1]
static const char *conjure_type<TEST::NineEnums>::tpeek() [T = TEST::NineEnums]
static const char *conjure_type<TEST::NineEnums1>::tpeek() [T = TEST::NineEnums1]
static const char *conjure_type<(anonymous namespace)::TEST1::NineEnums>::tpeek() [T = (anonymous namespace)::TEST1::NineEnums]
static const char *conjure_type<(anonymous namespace)::TEST1::NineEnums1>::tpeek() [T = (anonymous namespace)::TEST1::NineEnums1]
static const char *conjure_type<TEST::TEST1::NineEnums>::tpeek() [T = TEST::TEST1::NineEnums]
static const char *conjure_type<TEST::TEST1::NineEnums1>::tpeek() [T = TEST::TEST1::NineEnums1]
```
