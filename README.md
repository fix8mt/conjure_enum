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
//---------------------------------------------------------------------------------------->
<p align="center">
  <a href="https://www.fix8mt.com"><img src="assets/conjure_enum_logo.png" width="200"></a>
</p>

<h3 align="center">Lightweight header-only C++20 enum and typename reflection</h3>

---

[![clang](https://github.com/fix8mt/conjure_enum/actions/workflows/Ubuntu-clang-latest.yml/badge.svg)](https://github.com/fix8mt/conjure_enum/actions/workflows/Ubuntu-clang-latest.yml)
[![gcc](https://github.com/fix8mt/conjure_enum/actions/workflows/Ubuntu-gcc-latest.yml/badge.svg)](https://github.com/fix8mt/conjure_enum/actions/workflows/Ubuntu-gcc-latest.yml)
<a href="https://en.wikipedia.org/wiki/C%2B%2B20"><img src="assets/badgecpprequired.svg"></a>
<a href="https://opensource.org/license/mit"><img src="assets/badgemitlic.svg"></a>

# 1. Quick links
||**Link**|**Description**|
--|--|--
|1|[Implementation](include/fix8/conjure_enum.hpp)| For header implementation|
|2|[`conjure_enum` API and Examples](#3-api-and-examples-using-conjure_enum)| General examples|
|3|[`enum_bitset` API and Examples](#4-api-and-examples-using-enum_bitset)| Enhanced enum aware `std::bitset`|
|4|[`conjure_type` API and Examples](#5-api-and-examples-using-conjure_type)| Any type string extractor|
|5|[`fixed_string` API](#6-api-for-fixed-string)| Fixed string|
|6|[Building](#7-building)| How to build or include|
|7|[vcpkg](https://vcpkg.io/en/package/conjure-enum)| For vcpkg package|
|8|[Notes](#8-notes)| Notes on the implementation, limits, etc|
|9|[Compilers](#9-compiler-support)| Supported compilers|
|10|[Compiler issues](#10-compiler-issues)| Workarounds for various compiler issues|
|11|[Results of `std::source_location`](reference/source_location.md)| For implementation specific `std::source_location` results|
> [!TIP]
> Use the built-in [table of contents](https://github.blog/changelog/2021-04-13-table-of-contents-support-in-markdown-files/) to navigate this guide.
> Even better in [full read view](./README.md) of this page.
>
> For the latest cutting edge changes, see the [dev branch](https://github.com/fix8mt/conjure_enum/tree/dev).

---
# 2. Introduction
## a) Supercharge Your C++ Enums with This Lightweight Reflection Library!

Based on the awesome work in [`magic_enum`](https://github.com/Neargye/magic_enum)[^2] and [`boost::describe`](https://github.com/boostorg/describe),
this library offers a streamlined and powerful way to add reflection capabilities to your C++ enums and other types. We've optimized the core functionality,
focusing on the main features developers usually want. We've also added general purpose typename reflection for any type.

## b) Embracing C++20

`conjure_enum`[^1] takes full advantage of recently added C++20 features. We've leveraged the convenience of [`std::source_location`](https://en.cppreference.com/w/cpp/utility/source_location) and
unlocked the potential of [`constexpr` algorithms](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0202r3.html) and [concepts](https://en.cppreference.com/w/cpp/language/constraints).

## c) Highlights

- ***Single Header-Only***: No external dependencies, simplifying integration into your project
- ***Modern C++20***: Entirely `constexpr` for compile-time safety, efficiency and performance; no macros
- ***Broad Support***: Works with:
  - scoped and unscoped enums
  - enums with **aliases** and **gaps**
  - anonymous and named namespaced enums and types
  - custom enum ranges
- ***Easy to Use***: Class-based approach with intuitive syntax
- ***Convenient***: `enum_bitset` provides an enhanced enum aware `std::bitset` (see 3 above)
- ***Useful***: `conjure_type` gives you the type string of _any type!_ (see 4 above)
- ***Wide Compiler Compatibility***: Support for: (see 7 above)
  - GCC
  - Clang
  - MSVC
  - XCode/Apple Clang
- ***Confidence in Quality***: Includes comprehensive unit test suite for reliable functionality and profiling
- ***Expanded***: Enhanced API:
  - `add_scope`
  - `remove_scope`
  - `unscoped_string_to_enum`
  - `for_each_n`
  - `dispatch`
  - iterators and more!
- ***Transparency***: Compiler implementation variability fully documented, verifiable and reportable (see 9 above)

---
# 3. API and Examples using `conjure_enum`
All examples refer to the following enums:
```c++
enum class component { scheme, authority, userinfo, user, password, host, port, path=12, test=path, query, fragment };
enum component1 { scheme, authority, userinfo, user, password, host, port, path=12, query, fragment };
enum class numbers { zero, one, two, three, four, five, six, seven, eight, nine };
```

## a) `enum_to_string`
```c++
static constexpr std::string_view enum_to_string(T value, bool noscope=false);
```
Returns a `std::string_view` or empty if not found. Optionally passing `true` will remove scope in result if present.
`noscope` option ![](assets/notminimalred.svg).
```c++
auto name { conjure_enum<component>::enum_to_string(component::path) };
auto name_trim { conjure_enum<component>::enum_to_string(component::path, true) }; // optionally remove scope in result
auto alias_name { conjure_enum<component>::enum_to_string(component::test) }; // alias
auto noscope_name { conjure_enum<component1>::enum_to_string(path) };
std::cout << name << '\n' << name_trim << '\n' << alias_name << '\n' << noscope_name << '\n';
```
_output_
```CSV
component::path
path
component::path
path
```
### Aliases
Because all methods in `conjure_enum` are defined _within_ a `class` instead of individual template functions in a `namespace`, you can reduce your
typing with standard aliases:
```c++
using ec = conjure_enum<component>;
std::cout << std::format("\"{}\"\n", ec::enum_to_string(component::authority));
std::cout << std::format("\"{}\"\n", ec::enum_to_string(static_cast<component>(100)));
```
_output_
```CSV
"component::authority"
""
```
Also supplied is a template version of `enum_to_string`.
```c++
std::cout << std::format("\"{}\"\n", ec::enum_to_string<component::scheme>());
std::cout << std::format("\"{}\"\n", ec::enum_to_string<scheme>());
```
_output_
```CSV
"component::scheme"
"scheme"
```

## b) `string_to_enum`
```c++
static constexpr std::optional<T> string_to_enum(std::string_view str);
```
Returns a `std::optional<T>`. Empty if string was not valid. Use `std::optional<T>::value_or()` to set an error value
and avoid throwing an exception.
```c++
int value { static_cast<int>(conjure_enum<component>::string_to_enum("component::path").value()) };
int noscope_value { static_cast<int>(conjure_enum<component1>::string_to_enum("path").value()) };
int bad_value { static_cast<int>(conjure_enum<component>::string_to_enum("bad_string").value_or(component(100))) };
std::cout << value << '\n' << noscope_value << '\n' << bad_value << '\n';
```
_output_
```CSV
12
12
100 <-- invalid, error value
```
## c) `unscoped_string_to_enum` ![](assets/notminimalred.svg)
```c++
static constexpr std::optional<T> unscoped_string_to_enum(std::string_view str);
```
Same as `string_to_enum` except works with unscoped strings. Returns a `std::optional<T>`. Empty if string was not valid. Use `std::optional<T>::value_or()` to set an error value
and avoid throwing an exception.
```c++
int value { static_cast<int>(conjure_enum<component>::unscoped_string_to_enum("path").value()) };
int noscope_value { static_cast<int>(conjure_enum<component1>::string_to_enum("path").value()) };
int bad_value { static_cast<int>(conjure_enum<component>::string_to_enum("bad_string").value_or(component(100))) };
std::cout << value << '\n' << noscope_value << '\n' << bad_value << '\n';
```
_output_
```CSV
12
12
100 <-- invalid, error value
```
## d) `int_to_enum`
```c++
static constexpr std::optional<T> int_to_enum(int value);
```
Returns a `std::optional<T>`. Empty if value was not valid. Use `std::optional<T>::value_or()` to set an error value
and avoid throwing an exception.
```c++
int value { static_cast<int>(conjure_enum<component>::int_to_enum(12).value()) };
int noscope_value { static_cast<int>(conjure_enum<component1>::int_to_enum(12).value()) };
int bad_value { static_cast<int>(conjure_enum<component>::int_to_enum(100).value_or(component(100))) };
std::cout << value << '\n' << noscope_value << '\n' << bad_value << '\n';
```
_output_
```CSV
12
12
100 <-- invalid, error value
```
## e) `enum_to_int`, `enum_to_underlying`
```c++
static constexpr int enum_to_int(T value);
static constexpr std::underlying_type_t<T> enum_to_underlying(T value);
```
Returns an `int` or the `underlying` value for the given enum value. These are added for completeness. For unscoped enums
you can always just use the value like an int, or for scoped enums just `static_cast<int>` it first.
```c++
std::cout << conjure_enum<component>::enum_to_int(component::path) << '\n';
std::cout << conjure_enum<component>::enum_to_underlying(component::path) << '\n';
std::cout << conjure_enum<component1>::enum_to_int(path) << '\n';
std::cout << conjure_enum<component1>::enum_to_underlying(path) << '\n';
```
_output_
```CSV
12
12
12
12
```
## f) `count`
```c++
static constexpr std::size_t count();
```
Returns number of enumerations.
```c++
std::cout << conjure_enum<component>::count()  << '\n';
```
_output_
```CSV
10
```
## g) `names` ![](assets/notminimalred.svg)
```c++
static constexpr std::array<std::string_view, std::size_t> names;
```
This static member is generated for your type. It is a `std::array` of the `std::string_view` strings in enum order.
```c++
for(const auto ev : conjure_enum<component>::names) // scoped enum
   std::cout << ev << '\n';
for(const auto ev : conjure_enum<component1>::names) // unscoped enum
   std::cout << ev << '\n';
```
_output_
```CSV
component::scheme
component::authority
component::userinfo
component::user
component::password
component::host
component::port
component::path
component::query
component::fragment
scheme
authority
userinfo
user
password
host
port
path
query
fragment
```
## h) `unscoped_names` ![](assets/notminimalred.svg)
```c++
static constexpr std::array<std::string_view, std::size_t> unscoped_names;
```
This static member is generated for your type. It is a `std::array` of the `std::string_view` unscoped strings in enum order.
For unscoped enums is the same as `names` above.
```c++
for(const auto ev : conjure_enum<component>::unscoped_names) // scoped enum
   std::cout << ev << '\n';
std::cout << '\n';
for(const auto ev : conjure_enum<component1>::unscoped_names) // unscoped enum
   std::cout << ev << '\n';
```
_output_
```CSV
scheme
authority
userinfo
user
password
host
port
path
query
fragment

scheme
authority
userinfo
user
password
host
port
path
query
fragment
```
## i) `values`
```c++
static constexpr std::array<T, std::size_t> values;
```
This static member is generated for your type. It is a `std::array` of the `T` values in enum order.
```c++
for(const auto ev : conjure_enum<component>::values) // scoped enum
   std::cout << static_cast<int>(ev) << '\n';
```
_output_
```CSV
0
1
2
3
4
5
6
12
13
14
```
## j) `entries`, `sorted_entries`
```c++
static constexpr std::array<std::tuple<T, std::string_view>, std::size_t> entries;
static constexpr std::array<std::tuple<T, std::string_view>, std::size_t> sorted_entries;
```
These static members are generated for your type. They are `std::array` of tuples of `T` and `std::string_view`.
`sorted_entries` is the same as `entries` except the array is sorted by the `std::string_view` name.
```c++
using ec = conjure_enum<component>;
for(const auto [value, str] : ec::entries) // scoped enum
   std::cout << std::format("{:<2} {}\n", static_cast<int>(value), str);
std::cout << '\n';
for(const auto [value, str] : ec::sorted_entries) // scoped enum
   std::cout << std::format("{:<2} {}\n", static_cast<int>(value), str);
```
_output_
```CSV
0  component::scheme
1  component::authority
2  component::userinfo
3  component::user
4  component::password
5  component::host
6  component::port
12 component::path
13 component::query
14 component::fragment

1  component::authority
14 component::fragment
5  component::host
4  component::password
12 component::path
6  component::port
13 component::query
0  component::scheme
3  component::user
2  component::userinfo
```
## k) `scoped_entries`, `unscoped_entries` ![](assets/notminimalred.svg)
```c++
static constexpr std::array<std::tuple<std::string_view, std::string_view>, std::size_t> scoped_entries;
```
This static member is generated for your type. It is a `std::array` of a tuple of `std::string_view` pairs in enum order.
It contains pairs of unscoped and their scoped string version. This array is sorted by unscoped name.
For unscoped enums, these are identical.

`unscoped_entries` is the same except the pair is reversed.
```c++
for(const auto [a, b] : conjure_enum<component>::scoped_entries)
   std::cout << std::format("{:9} {}\n", a, b);
```
_output_
```CSV
authority component::authority
fragment  component::fragment
host      component::host
password  component::password
path      component::path
port      component::port
query     component::query
scheme    component::scheme
user      component::user
userinfo  component::userinfo
```
## l) `rev_scoped_entries` ![](assets/notminimalred.svg)
```c++
static constexpr std::array<std::tuple<std::string_view, std::string_view>, std::size_t> rev_scoped_entries;
```
Same as `scoped_entries` except reversed, sorted by scoped name. Use to lookup unscoped name.
## m) `index`
```c++
static constexpr std::optional<size_t> index(T value);
template<T e>
static constexpr std::optional<size_t> index();
```
Returns the index (position in 0 based array of values) of the supplied enum value as an `std::optional<size_t>`.
Empty if value was not valid. Use `std::optional<T>::value_or()` to set an error value
and avoid throwing an exception.
```c++
std::cout << conjure_enum<component>::index(component::password).value() << '\n';
std::cout << conjure_enum<component>::index(component(100)).value_or(100) << '\n';
std::cout << conjure_enum<component>::index<component::password>().value() << '\n';
std::cout << conjure_enum<component>::index<component(100)>().value_or(100) << '\n';
```
_output_
```CSV
4
100 <-- invalid, error value
4
100 <-- invalid, error value
```
## n) `contains`
```c++
static constexpr bool contains(T value);
static constexpr bool contains(std::string_view str);
template<T e>
static constexpr bool contains();
```
Returns `true` if the enum contains the given value or string.
```c++
std::cout << std::format("{}\n", conjure_enum<component>::contains(component::path));
std::cout << std::format("{}\n", conjure_enum<component1>::contains("nothing"));
std::cout << std::format("{}\n", conjure_enum<component>::contains<component::path>());
```
_output_
```CSV
true
false
true
```
## o) `for_each`, `for_each_n` ![](assets/notminimalred.svg)
```c++
template<typename Fn, typename... Args>
requires std::invocable<Fn&&, T, Args...>
[[maybe_unused]] static constexpr auto for_each(Fn&& func, Args&&... args);

template<typename Fn, typename C, typename... Args> // specialisation for member function with object
requires std::invocable<Fn&&, C, T, Args...>
[[maybe_unused]] static constexpr auto for_each(Fn&& func, C *obj, Args&&... args);

template<typename Fn, typename... Args>
requires std::invocable<Fn&&, T, Args...>
[[maybe_unused]] static constexpr auto for_each_n(int n, Fn&& func, Args&&... args);

template<typename Fn, typename C, typename... Args> // specialisation for member function with object
requires std::invocable<Fn&&, C, T, Args...>
[[maybe_unused]] static constexpr auto for_each_n(int n, Fn&& func, C *obj, Args&&... args);
```
Call supplied invocable for _each_ enum value. Similar to `std::for_each` except the first parameter of your invocable must accept an enum value (passed by `for_each`).
Optionally provide any additional parameters. Works with lambdas, member functions, functions etc. You can limit the number of calls to your
invocable by using the `for_each_n` version with the first parameter being the maximum number to call. The second version of `for_each` and `for_each_n` is intended to be used
when using a member function - the _second_ parameter passed by your call must be the `this` pointer of the object.
If you wish to pass a `reference` parameter, you must wrap it in `std::ref`.

Returns `std::bind(std::forward<Fn>(func), std::placeholders::_1, std::forward<Args>(args)...)`
or `std::bind(std::forward<Fn>(func), obj, std::placeholders::_1, std::forward<Args>(args)...)` which can be stored or immediately invoked.

See `enum_bitset::for_each` to iterate through a bitset.
```c++
conjure_enum<component>::for_each([](component val, int other)
{
   std::cout << static_cast<int>(val) << ' ' << other << '\n';
}, 10);
```
_output_
```CSV
0 10
1 10
2 10
3 10
4 10
5 10
6 10
12 10
13 10
14 10
```
Above example using `for_each_n`, limiting to 3:
```c++
conjure_enum<component>::for_each_n(3, [](component val, int other)
{
   std::cout << static_cast<int>(val) << ' ' << other << '\n';
}, 10);
```
_output_
```CSV
0 10
1 10
2 10
```
Example using returned object and additional reference parameter:
```c++
int total{};
auto myfunc { conjure_enum<component>::for_each([](component val, int other, int& tot)
{
   std::cout << static_cast<int>(val) << ' ' << other << '\n';
   tot += static_cast<int>(val);
}, 10, std::ref(total)) };
myfunc(component::fragment);
std::cout << total << '\n';
```
_output_
```CSV
0 10
1 10
2 10
3 10
4 10
5 10
6 10
12 10
13 10
14 10
14 10 <== invoked with returned object
74
```
Example with pointer to member function with additional parameters:
```c++
struct foo
{
   void process(component val, int offset, int& tot)
   {
      tot += offset + static_cast<int>(val);
   }
};
int total{};
foo bar;
conjure_enum<component>::for_each(&foo::process, &bar, 10, std::ref(total));
std::cout << total << '\n';
```
_output_
```CSV
160
```
## p) `dispatch` ![](assets/notminimalred.svg)
```c++
template<typename Fn>
static constexpr bool tuple_comp(const std::tuple<T, Fn>& pl, const std::tuple<T, Fn>& pr);

template<std::size_t I, typename R, typename Fn, typename... Args> // with not found value(nval) for return
requires std::invocable<Fn&&, T, Args...>
[[maybe_unused]] static constexpr R dispatch(T ev, R nval, const std::array<std::tuple<T, Fn>, I>& disp, Args&&... args);

template<std::size_t I, typename R, typename Fn, typename C, typename... Args> // specialisation for member function with not found value(nval) for return
requires std::invocable<Fn&&, C, T, Args...>
[[maybe_unused]] static constexpr R dispatch(T ev, R nval, const std::array<std::tuple<T, Fn>, I>& disp, C *obj, Args&&... args);

template<std::size_t I, typename Fn, typename... Args> // void func with not found call to last element
requires (std::invocable<Fn&&, T, Args...> && I > 0)
static constexpr void dispatch(T ev, const std::array<std::tuple<T, Fn>, I>& disp, Args&&... args);

template<std::size_t I, typename Fn, typename C, typename... Args> // specialisation for void member function with not found call to last element
requires (std::invocable<Fn&&, C, T, Args...> && I > 0)
static constexpr void dispatch(T ev, const std::array<std::tuple<T, Fn>, I>& disp, C *obj, Args&&... args);
```
With a given enum, search and call user supplied invocable. Use this method where complex event handling is required, allowing you to easily declare predefined invocable actions
for different enum values.

Where invocable returns a value, return this value or a user supplied "not found" value.
Where invocable is void, call user supplied "not found" invocable.
The first parameter of your invocable must accept an enum value (passed by `dispatch`).
Optionally provide any additional parameters. Works with lambdas, member functions, functions etc.

There are two versions of `dispatch` - the first takes an enum value, a 'not found' value, and a `std::array` of `std::tuple` of enum and invocable.
The second version takes an enum value, and a `std::array` of `std::tuple` of enum and invocable. The last element of the array is called if the enum is not found.
This version is intended for use with `void` return invocables.
The second version of each of the above is intended to be used when using a member function - the _first_ parameter passed after your array must be the `this` pointer of the object.
You can also use `std::bind` to bind the this pointer and any parameter placeholders when declaring your array.
If you wish to pass a `reference` parameter, you must wrap it in `std::ref`.

If you wish to provide a `constexpr` array, you will need to use a simple function prototype, since `std::function` is not constexpr (see unit tests for examples).
> [!IMPORTANT]
> Your `std::array` of `std::tuple` should be sorted by enum.
> The `dispatch` method performs a binary search on the array. Complexity for a sorted array is at most $2log_2(N)+O(1)$ comparisons.
> If the array is _not_ sorted, complexity is linear.
```c++
enum class directions { left, right, up, down, forward, backward, notfound=-1 };
static constexpr auto prn([](directions ev) { std::cout << conjure_enum<directions>::enum_to_string(ev) << '\n'; });
static constexpr auto tarr
{
   std::to_array<std::tuple<directions, void(*)(directions)>>
   ({
      { directions::left, prn },
      { directions::right, prn },
      { directions::up, prn },
      { directions::down, prn },
      { directions::backward, prn },
      { directions::notfound, [](directions ev) { std::cout << "not found: "; prn(ev); } }, // not found func
   })
};
conjure_enum<directions>::dispatch(directions::right, tarr);
conjure_enum<directions>::dispatch(directions::forward, tarr);
std::cout << conjure_enum<directions>::enum_to_int(directions::notfound) << '\n';
```
_output_
```CSV
directions::right
not found: directions::forward
-1
```
This example uses member functions:
```c++
struct foo
{
   int process(component val, int aint)
   {
      return aint * static_cast<int>(val);
   }
   int process1(component val, int aint)
   {
      return aint + static_cast<int>(val);
   }
};
constexpr auto tarr1
{
   std::to_array<std::tuple<component, int (foo::*)(component, int)>>
   ({
      { component::scheme, &foo::process },
      { component::port, &foo::process },
      { component::fragment, &foo::process1 },
   })
};
foo bar;
for (auto val : { component::scheme, component::path, component::port, component::fragment })
   std::cout << conjure_enum<component>::dispatch(val, -1, tarr1, &bar, 1000) << '\n';
```
_output_
```CSV
0
-1
6000
1015
```
## q) `is_scoped`
```c++
struct is_scoped : std::integral_constant<bool, requires
   { requires !std::is_convertible_v<T, std::underlying_type_t<T>>; }>{};
```
Returns `true` if the specified enum type is scoped.
```c++
std::cout << std::format("{}\n", conjure_enum<component>::is_scoped());
std::cout << std::format("{}\n", conjure_enum<component1>::is_scoped());
```
_output_
```CSV
true
false
```
## r) `is_valid`
```c++
template<T e>
static constexpr bool is_valid();
```
Returns `true` if enum value is valid.
```c++
std::cout << std::format("{}\n", conjure_enum<component>::is_valid<component::password>());
std::cout << std::format("{}\n", conjure_enum<component1>::is_valid<static_cast<component>(16)>());
```
_output_
```CSV
true
false
```
## s) `type_name` ![](assets/notminimalred.svg)
```c++
static constexpr std::string_view type_name();
```
Returns a `std::string_view` of `T`.
```c++
std::cout << conjure_enum<component>::type_name() << '\n';
std::cout << conjure_enum<component1>::type_name() << '\n';
```
_output_
```CSV
component
component1
```
## t) `remove_scope` ![](assets/notminimalred.svg)
```c++
static constexpr std::string_view remove_scope(std::string_view what);
```
Returns a `std::string_view` with scope removed; for unscoped returns unchanged
```c++
std::cout << conjure_enum<component>::remove_scope("component::path"sv) << '\n';
std::cout << conjure_enum<component>::remove_scope("path"sv) << '\n';
```
_output_
```CSV
path
path
```
## u) `add_scope` ![](assets/notminimalred.svg)
```c++
static constexpr std::string_view add_scope(std::string_view what);
```
Returns a `std::string_view` with scope added to the enum if the supplied enum string is valid but missing scope; for unscoped returns unchanged
```c++
std::cout << conjure_enum<component>::add_scope("path"sv) << '\n';
std::cout << conjure_enum<component1>::add_scope("path"sv) << '\n';
```
_output_
```CSV
component::path
path
```
## v) `has_scope` ![](assets/notminimalred.svg)
```c++
static constexpr bool has_scope(std::string_view what);
```
Returns `true` if the supplied string representation is scoped (and is valid).
```c++
std::cout << std::format("{}\n", conjure_enum<component>::has_scope("component::scheme"));
std::cout << std::format("{}\n", conjure_enum<component>::has_scope("scheme"));
std::cout << std::format("{}\n", conjure_enum<component1>::has_scope("scheme"));
```
_output_
```CSV
true
false
false
```
## w) `iterators` ![](assets/notminimalred.svg)
```c++
static constexpr auto cbegin();
static constexpr auto cend();
static constexpr auto crbegin();
static constexpr auto crend();
```
These methods return `const_iterator` and `const_reverse_iterator` respectively all from `entries`
defined above.
```c++
using en = conjure_enum<numbers>;
for (auto itr{en::cbegin()}; itr != en::cend(); ++itr)
   std::cout << static_cast<int>(std::get<0>(*itr)) << ' ' << std::get<1>(*itr) << '\n';
```
_output_
```CSV
0 numbers::zero
1 numbers::one
2 numbers::two
3 numbers::three
4 numbers::four
5 numbers::five
6 numbers::six
7 numbers::seven
8 numbers::eight
9 numbers::nine
```
## x) `iterator_adaptor` ![](assets/notminimalred.svg)
```c++
template<valid_enum T>
struct iterator_adaptor;
```
This class wraps `conjure_enum<T>::entries` allowing it to be used in range based for loops:
```c++
for (const auto pp : iterator_adaptor<numbers>())
   std::cout << static_cast<int>(std::get<0>(pp)) << '\n';
```
_output_
```CSV
0
1
2
3
4
5
6
7
8
9
```
## y) `front, back` ![](assets/notminimalred.svg)
```c++
static constexpr auto front();
static constexpr auto back();
```
These methods return `*cbegin()` and `*std::prev(cend())` respectively all from `entries`
defined above.
```c++
using en = conjure_enum<numbers>;
std::cout << static_cast<int>(std::get<0>(en::front())) << ' ' << std::get<1>(en::front()) << '\n';
std::cout << static_cast<int>(std::get<0>(en::back())) << ' ' << std::get<1>(en::back()) << '\n';
```
_output_
```CSV
0 numbers::zero
9 numbers::nine
```
## z) `ostream_enum_operator` ![](assets/notminimalred.svg)
```c++
template<typename CharT, typename Traits=std::char_traits<CharT>, valid_enum T>
constexpr std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, T value);
```
Provides `std::ostream` insertion for any enum. You just need to include
```c++
using ostream_enum_operator::operator<<;
```
Examples
```c++
using ostream_enum_operator::operator<<;
std::cout << '"' << component::host << '"' << '\n';
std::cout << '"' << component1::host << '"' << '\n';
std::cout << '"' << static_cast<component>(100) << '"' << '\n';
```
_output_
```CSV
"component::host"
"host"
"100"
```
## A) `epeek, tpeek`
```c++
static consteval const char *tpeek();
template<T e>
static consteval const char *epeek();
```
These functions return `std::source_location::current().function_name()` as `const char*` strings for the enum type or enum value.
The actual output is implementation dependent. See [Results of `source_location`](reference/source_location.md) for implementation specific `std::source_location` results.

The following code:
```c++
std::cout << conjure_enum<component>::tpeek() << '\n';
std::cout << conjure_enum<component>::epeek<component::scheme>() << '\n';
```
Generates this output with gcc:
```CSV
static consteval const char* FIX8::conjure_enum<T>::tpeek() [with T = component]
static consteval const char* FIX8::conjure_enum<T>::epeek() [with T e = component::path; T = component]
```

## B) `get_enum_min_value` and `get_enum_max_value`
```c++
static constexpr int get_enum_min_value();
static constexpr int get_enum_max_value();
```
These functions return the min and max enum range for the specified enum. If you have specialised `enum_range` then these values
will be reported (see below).
```c++
std::cout << conjure_enum<component>::get_enum_min_value() << '/' << conjure_enum<component>::get_enum_min_value() << '\n';
```
_output_
```CSV
-128/127
```
---
# 4. API and Examples using `enum_bitset`
`enum_bitset` is a convenient way of creating bitsets based on `std::bitset`. It uses your enum (scoped or unscoped)
for the bit positions (and names).
> [!WARNING]
> Your enum _must_ be continuous. The last value must be less than the count of enumerations.
> We decided on this restriction for both simplicity and practicality - bitsets only really make sense when represented in this manner.

> [!IMPORTANT]
> You must include
> ```C++
> #include <fix8/conjure_enum.hpp>
> #include <fix8/conjure_enum_bitset.hpp>
> ```

## a) Creating an `enum_bitset`
```c++
constexpr enum_bitset() = default;
constexpr enum_bitset(U bits);
constexpr enum_bitset(std::string_view from, bool anyscope=false,
   char sep='|', bool ignore_errors=true);

template<valid_bitset_enum... E>
constexpr enum_bitset(E... comp);

template<std::integral... I>
constexpr enum_bitset(I... comp);
```
You can use the enum values directly in your constructor. _No_ need to `|` them - this is assumed. Just supply them comma separated:
```c++
enum class numbers { zero, one, two, three, four, five, six, seven, eight, nine };
enum_bitset<numbers> b(numbers::zero, numbers::one, numbers::two, numbers::three);
std::cout << b << '\n';
```
_output_
```CSV
0000001111
```
You can use the underlying type as well:
```c++
enum_bitset<numbers> b(0,1,2,3);
std::cout << b << '\n';
```
_output_
```CSV
0000001111
```
You can use an `int` initialiser too:
```c++
enum_bitset<numbers> b(15);
std::cout << b << '\n';
```
_output_
```CSV
0000001111
```
You can even use a delimited string based on your enum names.
Optionally omit the scope and even specify your own delimiter (default is `|`).
Substrings are trimmed of whitespace before lookup.
```c++
enum_bitset<numbers> b("numbers::zero|numbers::one|numbers::two|numbers::three");
std::cout << b << '\n';
enum_bitset<numbers> b1("zero,one  ,two,  three", true, ',');
std::cout << b1 << '\n';
enum_bitset<numbers> b2("zero|one|two|three", true);
std::cout << b2 << '\n';
```
_output_
```CSV
0000001111
0000001111
0000001111
```
A typical use of the above is for parsing configuration bitsets. Here you can tell the constructor to throw an `std::invalid_argument`
if a substring is invalid by specifying `false` for `ignore_errors`:
```c++
try
{
   enum_bitset<numbers> b("zero,twenty,two,three", true, ',', false);
   std::cout << b << '\n';
}
catch(const std::invalid_argument& e)
{
   std::cerr << "exception: " << e.what() << '\n';
}
```
_output_
```CSV
exception: twenty
```
## b) Standard bit operators
All of the standard operators are supported. Assignment operators return a `enum_bitset&`, non-assignment operators return a `enum_bitset`.

| Operator | Description |
| :--- | :--- |
| `&` | binary AND |
| `\|` | binary OR |
| `^` | binary XOR |
| `~` | binary NOT (ones' complement)|
| `<<` | left shift |
| `>>` | right shift |
| `<<=` | left shift assign |
| `>>=` | right shift assign |
| `&=` | AND assign |
| `\|=` | OR assign |
| `^=` | XOR assign |

Operators work with enum values or integers:
```c++
enum_bitset<numbers> b(numbers::zero, numbers::one, numbers::two, numbers::three);
std::cout << b << '\n';
std::cout << (b & 0b111) << '\n';
b ^= numbers::two;
std::cout << b << '\n';
```
_output_
```
0000001111
0000000111
0000001011
```
## c) Standard accessors and mutators
All of the standard accessors and mutators are supported.
| Method | Description |
| :--- | :--- |
| `test` | test for bit(s)|
| `set` | set all or 1 bit, optionally set to off|
| `reset` | reset bits(s)|
| `flip` | flip bits(s) (ones' complement)|
| `to_ulong` | convert to `unsigned long` |
| `to_ullong` | convert to `unsigned long long` |
| `count` | count of bits on |
| `size` | number of bits in bitset |
| `operator[]` | test bit at position |
| `any` | return `true` if any bit is on |
| `all` | return `true` if all bits are on |
| `none` | return `true` if no bits are on |

Additional methods
| Method | Description |
| :--- | :--- |
| `set` | set all specified bits, templated |
| `reset` | reset all specified bits, templated |
| `any_of` | test for one or more bits, templated, function, types and underlyings |
| `all_of` | test for all specified bits, templated, function, types and underlyings |
| `none_of` | test for all specified bits set to off, templated, function, types and underlyings |
| `not_count` | complement of count, count of off bits |

Take a look at the [implementation](include/fix8/conjure_enum.hpp) for more detail on the various API functions available.
You can also review the unit test cases for examples of use.

All accessors and mutators work with enum values or integers as with operators. They also work with multiple values, either as template parameters or
as variadic arguments:
```c++
enum_bitset<numbers> eb;
eb.set<numbers::zero,numbers::two,numbers::five,numbers::nine>();
std::cout << eb << '\n';
std::cout << std::boolalpha << eb.all_of<numbers::zero,numbers::two,numbers::five,numbers::nine>() << '\n';
eb.reset<numbers::five,numbers::two>();
std::cout << std::boolalpha << eb.all_of(0, 2, 5, 9) << '\n';
std::cout << std::boolalpha << eb.any_of(0, 2, 5, 9) << '\n';
std::cout << std::boolalpha << eb.all_of(numbers::zero,numbers::nine) << '\n';
std::cout << eb << '\n';
eb.reset(numbers::nine)
std::cout << ec << '\n';
```
_output_
```
1000100101
true
false
true
true
1000000001
0000000001
```
## d) Other functions
### i. `operator bool`
```c++
constexpr operator bool() const;
```
Return true if _any_ bits are on.

```c++
if (enum_bitset<numbers> ec(15); ec)
   std::cout << ec << '\n';
```
_output_
```CSV
0001001111
```

### ii. `std::ostream& operator<<`, `to_string`
```c++
friend constexpr std::ostream& operator<<(std::ostream& os, const enum_bitset& what);
constexpr std::string to_string(char zero='0', char one='1') const;
```
Inserts default string representation into `std::ostream`.<br>
Returns a `std::string` representation of the bitset. Optionally specify which characters to use for `0` and `1`.

```c++
enum_bitset<numbers> ec(numbers::one,numbers::three,numbers::six);
std::cout << ec << '\n';
std::cout << ec.to_string('-', '+') << '\n';
```
_output_
```CSV
0001001010
---+--+-+-
```
### iii. `for_each`, `for_each_n`
```c++
template<typename Fn, typename... Args>
requires std::invocable<Fn&&, T, Args...>
[[maybe_unused]] constexpr auto for_each(Fn&& func, Args&&... args);

template<typename C, typename Fn, typename... Args> // specialisation for member function with object
requires std::invocable<Fn&&, C, T, Args...>
[[maybe_unused]] constexpr auto for_each(Fn&& func, C *obj, Args&&... args);

template<typename Fn, typename... Args>
requires std::invocable<Fn&&, T, Args...>
[[maybe_unused]] constexpr auto for_each_n(int n, Fn&& func, Args&&... args);

template<typename C, typename Fn, typename... Args> // specialisation for member function with object
requires std::invocable<Fn&&, C, T, Args...>
[[maybe_unused]] constexpr auto for_each_n(int n, Fn&& func, C *obj, Args&&... args);
```
Call supplied invocable for _every bit that is on_. Similar to `std::for_each` except first parameter of your invocable must accept an enum value (passed by `for_each`).
Optionally provide any additional parameters. Works with lambdas, member functions, functions etc. You can limit the number of calls to your
invocable by using the `for_each_n` version with the first parameter being the maximum number to call. The second version of `for_each` and `for_each_n` is intended to be used
when using a member function - the _second_ parameter passed by your call must be the `this` pointer of the object.
If you wish to pass a `reference` parameter, you must wrap it in `std::ref`.

Returns `std::bind(std::forward<Fn>(func), std::placeholders::_1, std::forward<Args>(args)...)` or
`std::bind(std::forward<Fn>(func), obj, std::placeholders::_1, std::forward<Args>(args)...)` which can be stored or immediately invoked.

To iterate over every bit regardless of whether it is on or not, use `conjure_enum<T>::for_each`.

Example using member function:
```c++
struct foo
{
   void printer(numbers val, std::ostream& ostr) const
   {
      ostr << conjure_enum<numbers>::enum_to_string(val) << '\n';
   }
};
enum_bitset<numbers> ec(numbers::zero,numbers::two,numbers::five,numbers::nine);
const foo bar;
ec.for_each(&foo::printer, &bar, std::ref(std::cout));
```
_output_
```CSV
numbers::zero
numbers::two
numbers::five
numbers::nine
```
Above example using `for_each_n`, limiting to 3:
```c++
ec.for_each_n(3, &foo::printer, &bar, std::ref(std::cout));
```
_output_
```CSV
numbers::zero
numbers::two
numbers::five
```

### iv. Using `conjure_enum::dispatch` with `enum_bitset`
Using an `enum_bitset` wth `conjure_enum::dispatch` can be a convenient way of iterating through a set of bits to call specific functions using `for_each`. The following demonstrates this:
```c++
const auto dd3
{
   std::to_array<std::tuple<numbers, std::function<void(numbers, int)>>>
   ({
      { numbers::one, [](numbers ev, int a)
         { std::cout << 1000 + a + conjure_enum<numbers>::enum_to_int(ev) << '\n'; } },
      { numbers::two, [](numbers ev, int a)
         { std::cout << 2000 + a + conjure_enum<numbers>::enum_to_int(ev) << '\n'; } },
      { numbers::three, [](numbers ev, int a)
         { std::cout << 3000 + a + conjure_enum<numbers>::enum_to_int(ev) << '\n'; } },
      { static_cast<numbers>(-1), [](numbers ev, [[maybe_unused]] int a)
         { std::cout << "not found: " << conjure_enum<numbers>::enum_to_int(ev) << '\n'; } }, // not found func
   })
};
enum_bitset<numbers>(1,2,3,5).for_each([](numbers val, const auto& arr, int num)
{
   conjure_enum<numbers>::dispatch(val, arr, num);
}, dd3, 100);
```
_output_
```CSV
1101
2102
3103
not found: 5
```

---
# 5. API and Examples using `conjure_type`
`conjure_type` is a general purpose class allowing you to extract a string representation of any typename.
The string will be stored statically by the compiler, so you can use the statically generated value `name` to obtain your type.
> [!IMPORTANT]
> You must include
> ```C++
> #include <fix8/conjure_enum.hpp>
> #include <fix8/conjure_type.hpp>
> ```

## a) `name`
This static member is generated for your type. It is a `fixed_string` but has a built-in `std::string_view` operator.
```c++
template<typename T>
class conjure_type;
static constexpr fixed_string name;
```

```c++
class foo;
std::cout << std::format("\"{}\"\n", conjure_type<foo>::name);
```
_output_
```CSV
"foo"
```
Works with aliases:
```c++
using test = std::map<std::size_t, std::string_view>;
using test1 = std::map<std::size_t, foo>;
std::cout << conjure_type<test>::name << '\n';
std::cout << conjure_type<test1>::name << '\n';
std::cout << conjure_type<std::underlying_type_t<numbers>>::name << '\n';
```
_output_
```CSV
std::map<long unsigned int, std::basic_string_view<char> >
std::map<long unsigned int, foo>
int
```
Works with its own types too:
```c++
std::cout << conjure_type<conjure_type<conjure_enum<numbers>>>::name << '\n';
```
_output_
```CSV
FIX8::conjure_type<FIX8::conjure_enum<numbers> >
```
If you need to explicitly obtain a `std::string_view`, use the `get()` method on `name` (not windows sorry):
```c++
auto fstrv { conjure_type<test>::name };
auto strv { conjure_type<test>::name.get() };
std::cout << conjure_type<decltype(fstrv)>::name << '\n';
std::cout << conjure_type<decltype(strv)>::name << '\n';
```
_output_
```CSV
fixed_string<58>
std::basic_string_view<char>
```
Alternatively you can use the `as_string_view()` method:
```c++
auto fstrv { conjure_type<test>::as_string_view() };
std::cout << conjure_type<decltype(fstrv)>::name << '\n';
```
_output_
```CSV
std::basic_string_view<char>
```

## b) `as_string_view`
Return the name as a `std::string_view`.
```c++
static constexpr std::string_view as_string_view();
```

## c) `tpeek`
```c++
static consteval const char *tpeek();
```

These functions return `std::source_location::current().function_name()` as `const char*` strings for type.
The actual output is implementation dependent. See [Results of `source_location`](reference/source_location.md) for implementation specific `std::source_location` results.

The following code:
```c++
std::cout << conjure_type<test>::tpeek() << '\n';
```
Generates this output with gcc:
```CSV
static consteval const char* FIX8::conjure_type<T>::tpeek() [with T = test]
```

---
# 6. API for `fixed_string`
`fixed_string` is a specialisation of `std::array` that provides statics storage for an ASCII zero (asciiz) string. The purpose of this class is to allow the
creation of `constexpr` strings with specfic storage, adding a trailing `0`. It is used by `conjure_enum` to store all strings. API is described below. Other uses of this class are possible.

## a) Creating a `fixed_string`
```c++
template<std::size_t N>
class fixed_string;
constexpr fixed_string(std::string_view sv);
```
Constructs a `fixed_string` from a `std::string_view`. Note the size must be passed as a template paramater.
```c++
std::string_view sv{"The rain in Spain"};
fixed_string<sv.size()> fs{sv};
```

## b) `get`
```c++
constexpr std::string_view get() const;
```
Returns the string as a `std::string_view`.

## c) `c_str`
```c++
constexpr const char *c_str() const;
```
Returns the string as a null terminated `const char *`.

## d) `operator std::string_view`
```c++
constexpr operator std::string_view() const;
```
Provides a `std::string_view` cast. Returns the string as a `std::string_view`.

## e) `operator[]`
```c++
constexpr char operator[](size_t idx) const;
```
Returns the character at the specifdined index. It is not range checked.

## f) `size`
```c++
constexpr std::size_t size() const;
```
Returns the size of the `fixed_string` including the null terminator.

## g) `std::ostream& operator<<`
```c++
std::ostream& operator<<(std::ostream& os, const fixed_string& what)
```
Provides an `ostream` insertor.

---
# 7. Building
This implementation is header only. Apart from standard C++20 includes there are no external dependencies needed in your application.
[Catch2](https://github.com/catchorg/Catch2.git) is used for the built-in unit tests.

## a) Obtaining the source, building the unittests and examples
### \*nix based environments
To clone and default build the test app, unit tests and the benchmark:
```bash
git clone https://github.com/fix8mt/conjure_enum.git
cd conjure_enum
mkdir build
cd build
cmake ..
make -j4
ctest (or make test)
```
By default all warnings are enabled. To prevent this, pass the following to cmake:
```cmake
cmake -DBUILD_ALL_WARNINGS=false ..
```
By default the unit tests are built (which will download Catch2). To prevent this, pass the following to cmake:
```cmake
cmake -DBUILD_UNITTESTS=false ..
```
To disable stripping of the executables:
```cmake
cmake -DBUILD_STRIP_EXE=false ..
```
### Windows environments
Create a new console project. Add the repo `https://github.com/fix8mt/conjure_enum.git` and clone the source.
Make sure you set the C++ language to C++20 in the project preferences. The project should build and run the unit tests
by default.

The package is also available on [vckpg](https://vcpkg.io/en/package/conjure-enum).

## b) Using in your application with cmake
In `CMakeLists.txt` set your include path to:
```cmake
include_directories([conjure_enum directory]/include)
# e.g.
set(cjedir /home/dd/prog/conjure_enum)
include_directories(${cjedir}/include)
```
and just include:
```c++
#include <fix8/conjure_enum.hpp>
```
in your application. Everything in this class is within the namespace `FIX8`, so you can add:
```c++
using namespace FIX8;
```

## c) Integrating `conjure_enum` in your project with cmake FetchContent
You can use cmake [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) to integrate `conjure_enum` with your project.
If your project was called `myproj` with the sourcefile `myproj.cpp` then...
```cmake
project(myproj)
add_executable (myproj myproj.cpp)
set_target_properties(myproj PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED true)
message(STATUS "Downloading conjure_enum...")
include(FetchContent)
FetchContent_Declare(conjure_enum GIT_REPOSITORY https://github.com/fix8mt/conjure_enum.git)
FetchContent_MakeAvailable(conjure_enum)
target_include_directories(myproj PRIVATE ${conjure_enum_SOURCE_DIR}/include)
```

## d) Reporting issues
Raise an [issue](https://github.com/fix8mt/conjure_enum/issues) on the github page.
The executable `srcloctest` should be built when you build the package by default. This application
does not use any of the `conjure_enum` library and is designed to report how your compiler handles `std::source_location`.
The actual output is implementation dependent. See [Results of `source_location`](reference/source_location.md) for implementation specific `std::source_location` results.
You should attach the output of this application with your issue.
> [!TIP]
> Use the `-m` switch with `srcloctest` to generate github markdown which you can paste directly into the issue.

```C++
$ ./srcloctest
Compiler: Clang: Ubuntu Clang 16.0.6 (23ubuntu4)
1. scoped enum
static const char *conjure_type<Namespace_Enum_Type>::tpeek() [T = Namespace_Enum_Type]
static const char *conjure_enum<Namespace_Enum_Type>::epeek() [T = Namespace_Enum_Type, e = Namespace_Enum_Type::Value]
static const char *conjure_enum<Namespace_Enum_Type>::epeek() [T = Namespace_Enum_Type, e = (Namespace_Enum_Type)100]

2. unscoped enum
static const char *conjure_type<Namespace_Enum_Type1>::tpeek() [T = Namespace_Enum_Type1]
static const char *conjure_enum<Namespace_Enum_Type1>::epeek() [T = Namespace_Enum_Type1, e = Value]
static const char *conjure_enum<Namespace_Enum_Type1>::epeek() [T = Namespace_Enum_Type1, e = (Namespace_Enum_Type1)100]

3. scoped enum in anonymous namespace
static const char *conjure_type<(anonymous namespace)::Anon_Enum_Type>::tpeek() [T = (anonymous namespace)::Anon_Enum_Type]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type>::epeek() [T = (anonymous namespace)::Anon_Enum_Type, e = (anonymous namespace)::Anon_Enum_Type::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type>::epeek() [T = (anonymous namespace)::Anon_Enum_Type, e = ((anonymous namespace)::Anon_Enum_Type)100]

4. unscoped enum in anonymous namespace
static const char *conjure_type<(anonymous namespace)::Anon_Enum_Type1>::tpeek() [T = (anonymous namespace)::Anon_Enum_Type1]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type1>::epeek() [T = (anonymous namespace)::Anon_Enum_Type1, e = (anonymous namespace)::Value]
static const char *conjure_enum<(anonymous namespace)::Anon_Enum_Type1>::epeek() [T = (anonymous namespace)::Anon_Enum_Type1, e = ((anonymous namespace)::Anon_Enum_Type1)100]

5. scoped enum in namespace
static const char *conjure_type<Namespace::Namespace_Enum_Type>::tpeek() [T = Namespace::Namespace_Enum_Type]
static const char *conjure_enum<Namespace::Namespace_Enum_Type>::epeek() [T = Namespace::Namespace_Enum_Type, e = Namespace::Namespace_Enum_Type::Value]
static const char *conjure_enum<Namespace::Namespace_Enum_Type>::epeek() [T = Namespace::Namespace_Enum_Type, e = (Namespace::Namespace_Enum_Type)100]

6. unscoped enum in namespace
static const char *conjure_type<Namespace::Namespace_Enum_Type1>::tpeek() [T = Namespace::Namespace_Enum_Type1]
static const char *conjure_enum<Namespace::Namespace_Enum_Type1>::epeek() [T = Namespace::Namespace_Enum_Type1, e = Namespace::Value]
static const char *conjure_enum<Namespace::Namespace_Enum_Type1>::epeek() [T = Namespace::Namespace_Enum_Type1, e = (Namespace::Namespace_Enum_Type1)100]

7. types in named and anonymous namespaces
static const char *conjure_type<Foo>::tpeek() [T = Foo]
static const char *conjure_type<Namespace::Namespace_Foo>::tpeek() [T = Namespace::Namespace_Foo]
static const char *conjure_type<(anonymous namespace)::Anon_Foo>::tpeek() [T = (anonymous namespace)::Anon_Foo]

8. other types
static const char *conjure_type<int>::tpeek() [T = int]
static const char *conjure_type<std::basic_string_view<char>>::tpeek() [T = std::basic_string_view<char>]
static const char *conjure_type<std::vector<std::tuple<int, char, std::basic_string_view<char>>>>::tpeek() [T = std::vector<std::tuple<int, char, std::basic_string_view<char>>>]

9. edge enum types
static const char *conjure_type<(anonymous namespace)::NineEnums>::tpeek() [T = (anonymous namespace)::NineEnums]
static const char *conjure_type<(anonymous namespace)::NineEnums1>::tpeek() [T = (anonymous namespace)::NineEnums1]
static const char *conjure_type<TEST::NineEnums>::tpeek() [T = TEST::NineEnums]
static const char *conjure_type<TEST::NineEnums1>::tpeek() [T = TEST::NineEnums1]
static const char *conjure_type<(anonymous namespace)::TEST1::NineEnums>::tpeek() [T = (anonymous namespace)::TEST1::NineEnums]
static const char *conjure_type<(anonymous namespace)::TEST1::NineEnums1>::tpeek() [T = (anonymous namespace)::TEST1::NineEnums1]
static const char *conjure_type<TEST::TEST1::NineEnums>::tpeek() [T = TEST::TEST1::NineEnums]
static const char *conjure_type<TEST::TEST1::NineEnums1>::tpeek() [T = TEST::TEST1::NineEnums1]
$
```

## e) Contributing
Contributions are welcome. Make your changes in [your fork on the dev branch](https://github.com/fix8mt/conjure_enum/tree/dev) and open a pull request from there. PRs to
master will not be considered.

---
# 8. Notes
## a) enum limits
### i. `FIX8_CONJURE_ENUM_MIN_VALUE`, `FIX8_CONJURE_ENUM_MAX_VALUE`
These are set by default unless you override them by defining them in your application. They are the global range default for enums using `conjure_enum`.
> [!IMPORTANT]
> If you want to define these values they must appear _before_ you include `conjure_enum.hpp`.

The following are the default settings:
```c++
#if not defined FIX8_CONJURE_ENUM_MIN_VALUE
# define FIX8_CONJURE_ENUM_MIN_VALUE -128
#endif
#if not defined FIX8_CONJURE_ENUM_MAX_VALUE
# define FIX8_CONJURE_ENUM_MAX_VALUE 127
#endif
```
These definitions set the minimum and maximum enum values that are supported. You can adjust them to suit your requirements but for most use cases the defaults are sufficient.
> [!TIP]
> You can reduce compile times in some circumstances by narrowing the range of `FIX8_CONJURE_ENUM_MIN_VALUE` and `FIX8_CONJURE_ENUM_MAX_VALUE`. For example
> if your enums are only within the range of say `0-16` you can set `FIX8_CONJURE_ENUM_MIN_VALUE` and `FIX8_CONJURE_ENUM_MAX_VALUE` to `0` and `16` respectively. If the range is _too_ narrow
> `conjure_enum` will **ignore enum values outside your range**.

> [!TIP]
> If you wish to set ranges on a per enum basis, use `enum_range` (see below).

### ii. using `enum_range`
```c++
template<valid_enum T>
struct enum_range
{
   static constexpr int min{FIX8_CONJURE_ENUM_MIN_VALUE}, max{FIX8_CONJURE_ENUM_MAX_VALUE};
};
```
The `min` and `max` values are used to set the range of enum values for enums in `conjure_enum`. As shown above, the default values will be
`FIX8_CONJURE_ENUM_MIN_VALUE` and `FIX8_CONJURE_ENUM_MAX_VALUE`.

You can specialise this class to override the defaults and set your own range on a per enum basis:
```c++
enum class range_test { first, second, third, fourth, fifth, sixth, seventh, eighth };
template<>
struct FIX8::enum_range<range_test>
{
   static constexpr int min{0}, max{7};
};
static_assert(conjure_enum<range_test>::get_enum_min_value() == 0);
static_assert(conjure_enum<range_test>::get_enum_max_value() == 7);
```
### iii. `FIX8_CONJURE_ENUM_SET_RANGE_INTS`, `FIX8_CONJURE_ENUM_SET_RANGE`
For convenience, two macros are provided to make it easier to set custom ranges.
```c++
#define FIX8_CONJURE_ENUM_SET_RANGE_INTS(ec,min_int,max_int)...
#define FIX8_CONJURE_ENUM_SET_RANGE(min_enum,max_enum)...
```
The first macro takes an enum typename followed by a lower and upper int range value.
The second macro takes a lower and upper enum value. For example:
```c++
FIX8_CONJURE_ENUM_SET_RANGE_INTS(numbers, 0, 7)
```
```c++
FIX8_CONJURE_ENUM_SET_RANGE(numbers::first, numbers::eighth)
```

## b) Choosing the minimal build
```c++
#define FIX8_CONJURE_ENUM_MINIMAL
```
You can select a minimal version of `conjure_enum` by defining `FIX8_CONJURE_ENUM_MINIMAL` _before_ you include `conjure_enum.hpp`

This limits the API to a more basic set of functionality. This will reduce compile times.
Static structures and API calls that will be excluded are:
```c++
scoped_entries
unscoped_entries
rev_scoped_entries
names
unscoped_names
remove_scope
add_scope
unscoped_string_to_enum
for_each,for_each_n
type_name
dispatch
iterators
enum_to_string //noscope option not available
```
These are marked ![](assets/notminimalred.svg) in the API documentation above.

## c) Continuous enum optimization
```c++
#define FIX8_CONJURE_ENUM_IS_CONTINUOUS
```
If your enum(s) are continuous (no gaps) you can enable this compiler optimization
by defining `FIX8_CONJURE_ENUM_IS_CONTINUOUS` _before_ you include `conjure_enum.hpp`.
Our testing shows a reduction in overall compile times. All enums using `conjure_enum.hpp` in the current compilation unit must be continuous.

## d) Anonymous enum optimization
```c++
#define FIX8_CONJURE_ENUM_NO_ANON
```
If your enum(s) are not within any anonymous namespaces (rarely used for this purpose), you can enable this compiler optimization
by defining `FIX8_CONJURE_ENUM_NO_ANON` _before_ you include `conjure_enum.hpp`.
Our testing shows a reduction in overall compile times. All enums using `conjure_enum.hpp` in the current compilation unit must be continuous.

## e) Enable all optimizations
```c++
#define FIX8_CONJURE_ENUM_ALL_OPTIMIZATIONS
```
You can enable all optimizations described above by defining `FIX8_CONJURE_ENUM_ALL_OPTIMIZATIONS` _before_ you include `conjure_enum.hpp`.
This is the equivalent of defining:
```c++
#define FIX8_CONJURE_ENUM_MINIMAL
#define FIX8_CONJURE_ENUM_IS_CONTINUOUS
#define FIX8_CONJURE_ENUM_NO_ANON
```

## f) Class `conjure_enum` is not constructible
All methods in this class are _static_. You cannot instantiate an object of this type. The same goes for `conjure_type`.

## g) It's not _real_ reflection
This library provides a workaround (hack :smirk:) to current limitations of C++. There are proposals out there for future versions of the language that will provide proper reflection.
See [Reflection TS](https://en.cppreference.com/w/cpp/experimental/reflect) and [Reflection for C++26](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2996r0.html)
for examples of some of these.

## h) Use of `std::string_view`
All of the generated static strings and generated static tables obtained by `std::source_location` use the library defined `fixed_string`. No string copying is done at runtime, resulting in
a single static string in your application. All `conjure_enum` methods that return strings _only_ return `std::string_view`.
To demonstrate this, lets look at the supplied test application `statictest`:
```c++
#include <iostream>
#define FIX8_CONJURE_ENUM_MINIMAL
#include <fix8/conjure_enum.hpp>
enum class component : int { scheme, authority, userinfo, user, password, host, port, path, query, fragment };
FIX8_CONJURE_ENUM_SET_RANGE(component::scheme, component::fragment)
int main(void)
{
   for(const auto& [a, b] : conjure_enum<component>::entries)
      std::cout << conjure_enum<component>::enum_to_int(a) << ' ' << b << '\n';
   std::cout << static_cast<int>(conjure_enum<component>::string_to_enum("component::path").value()) << '\n';
   std::cout << conjure_enum<component>::get_enum_min_value() << '/' << conjure_enum<component>::get_enum_max_value() << '\n';
   return 0;
}
```
_output_
```CSV
$ ./statictest
0 component::scheme
1 component::authority
2 component::userinfo
3 component::user
4 component::password
5 component::host
6 component::port
7 component::path
8 component::query
9 component::fragment
component::scheme
component::authority
component::userinfo
component::user
component::password
component::host
component::port
component::path
component::query
component::fragment
7
0/9
$
```

The default build of `statictest` performs a [strip](https://en.wikipedia.org/wiki/Strip_(Unix)) on the executable.
Then we run [strings](https://en.wikipedia.org/wiki/Strings_(Unix)) on the executable.
<details><summary><i>shell output</i></summary>
<p>
<pre>$ strings statictest
/lib64/ld-linux-x86-64.so.2
__gmon_start__
_ITM_deregisterTMCloneTable
_ITM_registerTMCloneTable
_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c
_ZNSolsEi
_ZSt21ios_base_library_initv
_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
_ZNSo3putEc
_ZSt4cout
__stack_chk_fail
__libc_start_main
__cxa_finalize
libstdc++.so.6
libc.so.6
GLIBC_2.4
GLIBC_2.34
GLIBC_2.2.5
GLIBCXX_3.4.32
GLIBCXX_3.4.9
GLIBCXX_3.4
ATUH
[]A\A]A^
PTE1
u+UH
<i><b>component::fragment</b></i>
<i><b>component::query</b></i>
<i><b>component::path</b></i>
<i><b>component::port</b></i>
<i><b>component::host</b></i>
<i><b>component::password</b></i>
<i><b>component::user</b></i>
<i><b>component::userinfo</b></i>
<i><b>component::authority</b></i>
<i><b>component::scheme</b></i>
9*3$"
GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0
.shstrtab
.interp
.note.gnu.property
.note.gnu.build-id
.note.ABI-tag
.gnu.hash
.dynsym
.dynstr
.gnu.version
.gnu.version_r
.rela.dyn
.rela.plt
.init
.plt.got
.plt.sec
.text
.fini
.rodata
.eh_frame_hdr
.eh_frame
.init_array
.fini_array
.data.rel.ro
.dynamic
.data
.bss
.comment
$
</pre></p></details>

It can be observed that there is only _one_ copy of the scoped enum value string in the executable.

## i) Compilation profiling (Clang)
You can profile the compile time for Clang (other compilers TBA). Firstly install [ClangBuildAnalyzer](https://github.com/aras-p/ClangBuildAnalyzer).
Then configure `conjure_enum` with:
```CMake
cmake -DBUILD_CLANG_PROFILER=true ..
```
You can follow the instructions given on the `ClangBuildAnalyzer` page to run. Alternatively after you build the included program `cbenchmark`,
run the included script [cbenchmark.sh](examples/cbenchmark.sh). The script expects the following environment variables:

| Variable | Description |
| :--- | :--- |
| `ClangBuildAnalyzerLoc` | directory where ClangBuildAnalyzer can be found|
| `ArtifactLoc` | directory where `conjure_enum` is built|

For example, if `ClangBuildAnalyzer` was built in `~/prog/ClangBuildAnalyzer/build` and your `conjure_enum` build was in `./build_clang`, then you
would run the script from the `conjure_enum` directory as follows:
```bash
ClangBuildAnalyzerLoc=~/prog/ClangBuildAnalyzer/build ArtifactLoc=build_clang examples/cbenchmark.sh
```
The results will be printed to the screen. For example:
<details><summary><i>output</i></summary>
<p>

```CSV
Processing all files and saving to 'cbenchmark.dat'...
  done in 0.0s. Run 'ClangBuildAnalyzer --analyze cbenchmark.dat' to analyze it.
Analyzing build trace from 'cbenchmark.dat'...
**** Time summary:
Compilation (2 times):
  Parsing (frontend):            0.7 s
  Codegen & opts (backend):      0.0 s

**** Files that took longest to parse (compiler frontend):
   662 ms: build_clang/CMakeFiles/cbenchmark.dir/examples/cbenchmark.cpp.o

**** Files that took longest to codegen (compiler backend):
    18 ms: build_clang/CMakeFiles/cbenchmark.dir/examples/cbenchmark.cpp.o

**** Templates that took longest to instantiate:
   290 ms: FIX8::conjure_enum<std::errc> (1 times, avg 290 ms)
   140 ms: FIX8::conjure_enum<std::errc>::_entries<0UL, 1UL, 2UL, 3UL, 4UL, 5UL... (1 times, avg 140 ms)
    82 ms: FIX8::conjure_enum<std::errc>::_values<0UL, 1UL, 2UL, 3UL, 4UL, 5UL,... (1 times, avg 82 ms)
     8 ms: FIX8::conjure_enum<std::errc>::_sorted_entries (1 times, avg 8 ms)
     7 ms: std::reverse_iterator<std::_Bit_iterator> (1 times, avg 7 ms)
     6 ms: std::sort<std::tuple<std::errc, std::basic_string_view<char>> *, boo... (1 times, avg 6 ms)
     6 ms: std::__sort<std::tuple<std::errc, std::basic_string_view<char>> *, _... (1 times, avg 6 ms)
     6 ms: std::reverse_iterator<std::_Bit_const_iterator> (1 times, avg 6 ms)
     5 ms: std::unordered_map<int, int> (1 times, avg 5 ms)
     5 ms: std::__introsort_loop<std::tuple<std::errc, std::basic_string_view<c... (1 times, avg 5 ms)
     3 ms: std::array<std::tuple<std::errc, std::basic_string_view<char>>, 47> (1 times, avg 3 ms)
     3 ms: std::_Hashtable<int, std::pair<const int, int>, std::allocator<std::... (1 times, avg 3 ms)
     3 ms: FIX8::conjure_enum<std::errc>::enum_to_string (1 times, avg 3 ms)
     3 ms: std::tuple<std::basic_string_view<char>, char, std::basic_string_vie... (1 times, avg 3 ms)
     2 ms: std::__unguarded_partition_pivot<std::tuple<std::errc, std::basic_st... (1 times, avg 2 ms)
     2 ms: std::tuple<std::errc, std::basic_string_view<char>> (1 times, avg 2 ms)
     2 ms: std::tuple<std::errc, std::basic_string_view<char>>::tuple<const std... (1 times, avg 2 ms)
     2 ms: FIX8::conjure_enum<std::errc>::_enum_name<std::errc::inappropriate_i... (1 times, avg 2 ms)
     2 ms: std::__move_median_to_first<std::tuple<std::errc, std::basic_string_... (1 times, avg 2 ms)
     2 ms: std::iter_swap<std::tuple<std::errc, std::basic_string_view<char>> *... (1 times, avg 2 ms)
     2 ms: std::__partial_sort<std::tuple<std::errc, std::basic_string_view<cha... (1 times, avg 2 ms)
     2 ms: FIX8::conjure_enum<std::errc>::_enum_name<std::errc::too_many_files_... (1 times, avg 2 ms)
     2 ms: std::__heap_select<std::tuple<std::errc, std::basic_string_view<char... (1 times, avg 2 ms)
     2 ms: std::tuple<std::basic_string_view<char>, char, std::basic_string_vie... (1 times, avg 2 ms)
     2 ms: FIX8::conjure_enum<std::errc>::_enum_name<std::errc::no_such_file_or... (1 times, avg 2 ms)
     2 ms: FIX8::conjure_enum<std::errc>::_enum_name<std::errc::operation_not_p... (1 times, avg 2 ms)
     2 ms: FIX8::conjure_enum<std::errc>::_enum_name<std::errc::no_message_avai... (1 times, avg 2 ms)
     2 ms: FIX8::conjure_enum<std::errc>::_enum_name<std::errc::operation_would... (1 times, avg 2 ms)
     2 ms: FIX8::conjure_enum<std::errc>::_enum_name<std::errc::no_space_on_dev... (1 times, avg 2 ms)
     2 ms: std::__make_heap<std::tuple<std::errc, std::basic_string_view<char>>... (1 times, avg 2 ms)

**** Template sets that took longest to instantiate:
   290 ms: FIX8::conjure_enum<$> (1 times, avg 290 ms)
   140 ms: FIX8::conjure_enum<$>::_entries<$> (1 times, avg 140 ms)
    82 ms: FIX8::conjure_enum<$>::_values<$> (1 times, avg 82 ms)
    78 ms: FIX8::conjure_enum<$>::_enum_name<$> (47 times, avg 1 ms)
    55 ms: FIX8::conjure_enum<$>::_is_valid<$> (72 times, avg 0 ms)
    38 ms: FIX8::conjure_enum<$>::_get_name<$> (47 times, avg 0 ms)
    15 ms: std::tuple<$>::tuple<$> (20 times, avg 0 ms)
    13 ms: std::reverse_iterator<$> (2 times, avg 6 ms)
     8 ms: FIX8::conjure_enum<$>::_sorted_entries (1 times, avg 8 ms)
     7 ms: std::basic_string<$> (5 times, avg 1 ms)
     6 ms: std::sort<$> (1 times, avg 6 ms)
     6 ms: std::__sort<$> (1 times, avg 6 ms)
     5 ms: std::tuple<$> (2 times, avg 2 ms)
     5 ms: std::_Hashtable<$> (2 times, avg 2 ms)
     5 ms: std::unordered_map<$> (1 times, avg 5 ms)
     5 ms: std::__introsort_loop<$> (1 times, avg 5 ms)
     4 ms: std::array<$> (2 times, avg 2 ms)
     4 ms: std::basic_string<$>::_M_construct<$> (4 times, avg 1 ms)
     3 ms: FIX8::conjure_enum<$>::enum_to_string (1 times, avg 3 ms)
     3 ms: std::__and_<$> (4 times, avg 0 ms)
     2 ms: std::__unguarded_partition_pivot<$> (1 times, avg 2 ms)
     2 ms: std::__move_median_to_first<$> (1 times, avg 2 ms)
     2 ms: std::iter_swap<$> (1 times, avg 2 ms)
     2 ms: std::__partial_sort<$> (1 times, avg 2 ms)
     2 ms: std::copy<$> (3 times, avg 0 ms)
     2 ms: std::__heap_select<$> (1 times, avg 2 ms)
     2 ms: std::_Tuple_impl<$> (2 times, avg 1 ms)
     2 ms: std::__make_heap<$> (1 times, avg 2 ms)
     1 ms: std::optional<$> (1 times, avg 1 ms)
     1 ms: std::to_array<$> (1 times, avg 1 ms)

**** Functions that took longest to compile:
     2 ms: test_conjure_enum(std::errc) (/home/davidd/prog/conjure_enum_tclass/examples/cbenchmark.cpp)

**** Function sets that took longest to compile / optimize:

**** Expensive headers:
181 ms: /home/davidd/prog/conjure_enum_tclass/include/fix8/conjure_enum.hpp (included 1 times, avg 181 ms), included via:
  1x: <direct include>

173 ms: /usr/include/c++/14/system_error (included 1 times, avg 173 ms), included via:
  1x: <direct include>

  done in 0.0s.
```

</p></details>

---
# 9. Compiler support
| Compiler | Version(s) | Notes | Unsupported |
| :--- | :--- | :--- | ---: |
| [gcc](https://gcc.gnu.org/projects/cxx-status.html) | `11`, `12`, `13`, `14`| `std::format` not complete in `11`, `12` | `<= 10` |
| [clang](https://clang.llvm.org/cxx_status.html) | `15`, `16`, `17`, `18`| Catch2 needs `cxx_std_20` in `15` | `<= 14` |
| [msvc](https://learn.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance) | `16`, `17` | Visual Studio 2019,2022, latest `17.10.5`| `<= 16.9`|
| [xcode](https://developer.apple.com/support/xcode/) | `15` | Apple LLVM 15.0.0, some issues with `constexpr`, workarounds| `<= 14`|

# 10. Compiler issues
| Compiler | Version(s) | Issues | Workaround |
| :--- | :--- | :--- | ---: |
| clang | `16`, `17`, `18`| Compiler reports integers outside valid range [x,y]| specify underlying type when declaring enum eg. `enum class foo : int` |

[^1]: &copy; 2024 Fix8 Market Technologies Pty Ltd, David L. Dight.
  Logo by [Adrian An](mailto:adrian.an[at]mac.com).
[^2]: &copy; 2019 - 2024 Daniil Goncharov

#
<p align="center">
  <a href="https://www.fix8mt.com"><img src="assets/fix8mt_Master_Logo_Green_Trans.png" width="120"></a>
</p>

