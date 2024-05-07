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
<p align="center">
  <a href="https://www.fix8mt.com"><img src="https://github.com/fix8mt/conjure_enum/blob/master/assets/fix8mt_Master_Logo_Green_Trans.png" width="200"></a>
</p>

# `conjure_enum`

### Lightweight header-only C++20 enum and type reflection

---
[![clang](https://github.com/fix8mt/conjure_enum/actions/workflows/Ubuntu-clang-latest.yml/badge.svg)](https://github.com/fix8mt/conjure_enum/actions/workflows/Ubuntu-clang-latest.yml)
[![gcc](https://github.com/fix8mt/conjure_enum/actions/workflows/Ubuntu-gcc-latest.yml/badge.svg)](https://github.com/fix8mt/conjure_enum/actions/workflows/Ubuntu-gcc-latest.yml)
<a href="https://en.wikipedia.org/wiki/C%2B%2B20"><img src="https://github.com/fix8mt/conjure_enum/blob/master/assets/badgecpprequired.svg"></a>
<a href="https://opensource.org/license/mit"><img src="https://github.com/fix8mt/conjure_enum/blob/master/assets/badgemitlic.svg"></a>

# 1. Quick links
|**Link**|**Description**|
--|--
|[Here](https://github.com/fix8mt/conjure_enum/blob/master/include/fix8/conjure_enum.hpp)| For implementation|
|[API and Examples](#3-api-and-examples)| General examples|
|[API and Examples for `enum_bitset`](#4-api-and-examples-using-enum_bitset)| `std::bitset` replacement|
|[API and Examples for `conjure_type`](#5-api-and-examples-using-conjure_type)| any type string extractor|
|[Building](#6-building)| How to build or include|
|[Notes](#7-notes)| Notes on the implementation, limits, etc|
|[Compilers](#8-compiler-support)| Supported compilers|
|[Compiler issues](#9-compiler-issues)| Workarounds for various compiler issues|
|[Review `std::source_location`](https://github.com/fix8mt/conjure_enum/blob/master/reference/source_location.md)| For implementation specific `source_location` results|
> [!TIP]
> Use the built-in [table of contents](https://github.blog/changelog/2021-04-13-table-of-contents-support-in-markdown-files/) to navigate this guide.

---
# 2. Introduction
## a) Supercharge Your C++ Enums with This Lightweight Reflection Library!

Based on the awesome work of [`magic_enum`](https://github.com/Neargye/magic_enum)[^2] and [`boost::desribe`](https://github.com/boostorg/describe),
this library offers a streamlined and powerful way to add reflection capabilities to your C++ enums and other types. We've optimized the core functionality,
focusing on the main features developers usually need while enhancing and expanding them for a more efficient and expressive experience. We've also
added general purpose type reflection for any type.

## b) Embrace the Future with C++20

`conjure_enum`[^1] takes full advantage of the latest C++20 features. We've leveraged the convenience of `std::source_location` and
unlocked the potential of `constexpr` algorithms and concepts. This translates to:

- ***Improved Performance***  Optimized code for faster and smoother operation - get your compiler to do more!
- ***Enhanced Developer Experience***  Write cleaner, more concise, and more powerful C++ code.

## c) Key Benefits

- ***Lightweight***:  Designed for performance without unnecessary overhead.
- ***Single Header-Only***:  No external dependencies, simplifying integration into your project.
- ***Modern C++20***:  Entirely `constexpr` for compile-time safety, efficiency and performance.
- ***Broad Support***:  Works with scoped and unscoped enums, enum **aliases** and even with **gaps**.
- ***Simple & Easy to Use***:  Class-based approach with intuitive syntax.
- ***Convenient***:  `enum_bitset` offers an enhanced `std::bitset`.
- ***Useful***:  `conjure_type` lets you obtain the type string of _any type!_
- ***Wide Compiler Compatibility***:  Supports GCC, Clang, MSVC and XCode/Clang; `x86_64`, `AArch64`
- ***Confidence in Quality***:  Includes comprehensive unit tests for reliable functionality.
- ***Expanded***:  Enhanced API such as `add_scope`, `remove_scope`, `unscoped_string_to_enum`, iterators and more!

---
# 3. API and Examples
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
Because all methods in `conjure_enum` are within a `class` instead of individual template functions in a `namespace`, you can reduce your
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
## c) `unscoped_string_to_enum`
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
Returns an `int` or the `underlying` value for the given enum value.
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
## g) `names`
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
## h) `unscoped_names`
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
## k) `scoped_entries`
```c++
static constexpr std::array<std::tuple<std::string_view, std::string_view>, std::size_t> scoped_entries;
```
This static member is generated for your type. It is a `std::array` of a tuple of `std::string_view` pairs in enum order.
It contains pairs of unscoped and their scoped string version. This array is sorted by unscoped name.
For unscoped enums, these are identical.
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
## l) `rev_scoped_entries`
```c++
static constexpr std::array<std::tuple<std::string_view, std::string_view>, std::size_t> rev_scoped_entries;
```
Same as `scoped_entries` except reversed, sorted by scoped name. Use to lookup unscoped name.
## m) `contains`
```c++
static constexpr bool contains(T value);
static constexpr bool contains(std::string_view str);
```
Returns `true` if the enum contains the given value or string.
```c++
std::cout << std::format("{}\n", conjure_enum<component>::contains(component::path));
std::cout << std::format("{}\n", conjure_enum<component1>::contains("nothing"));
```
_output_
```CSV
true
false
```
## n) `for_each`
```c++
template<typename Fn, typename... Args>
requires std::invocable<Fn&&, T, Args...>
[[maybe_unused]] static constexpr auto for_each(Fn&& func, Args&&... args);

template<typename Fn, typename C, typename... Args> // specialisation for member function with object
requires std::invocable<Fn&&, C, T, Args...>
[[maybe_unused]] static constexpr auto for_each(Fn&& func, C *obj, Args&&... args);
```
Call supplied invocable for _each_ enum value. Similar to `std::for_each` except the first parameter of your invocable must accept an enum value (passed by `for_each`).
Optionally provide any additional parameters. Works with lambdas, member functions, functions etc. The second version is intended to be used
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
## o) `is_scoped`
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
## p) `is_valid`
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
## q) `type_name`
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
## r) `remove_scope`
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
## s) `add_scope`
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
## t) `has_scope`
```c++
static constexpr bool has_scope(std::string_view what);
```
Returns `true` if the supplied string is scoped (and is valid).
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
## u) `iterators`
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
## v) `iterator_adaptor`
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
## w) `front, back`
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
## x) `ostream_enum_operator`
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
## y) `epeek, tpeek`
```c++
static consteval const char *tpeek();
template<T e>
static consteval const char *epeek();
```
These functions return `std::source_location::current().function_name()` as `const char*` strings for the enum type or enum value.
The actual output is implementation dependent.
> [!TIP]
> If you want to report an issue please include the output of these methods.

The following code:
```c++
std::cout << conjure_enum<component>::tpeek() << '\n';
std::cout << conjure_enum<component>::epeek<component::scheme>() << '\n';
```
Generates this output with gcc:
```CSV
static consteval const char* FIX8::conjure_enum<T>::epeek() [with T e = component::path; T = component]
static consteval const char* FIX8::conjure_enum<T>::tpeek() [with T = component]
```
---
# 4. API and Examples using `enum_bitset`
`enum_bitset` is a convenient way of creating bitsets based on `std::bitset`. It uses your enum (scoped or unscoped)
for the bit positions (and names).
> [!NOTE]
> Your enum should be continuous. The last value must be less than the count of enumerations.

We decided on this restriction for both simplicity and practicality - bitsets only really make sense when represented in this manner.

## a) Creating an `enum_bitset`
```c++
constexpr enum_bitset() = default;
constexpr enum_bitset(U bits);
constexpr enum_bitset(std::string_view from, bool anyscope=false,
   char sep='|', bool ignore_errors=true);

template<valid_enum... E>
constexpr enum_bitset(E... comp);

template<std::integral... I>
constexpr enum_bitset(I... comp);
```
You can use the enum values directly in your constructor. _No_ need to `|` them - this is assumed. Just supply them comma separated:
```c++
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
if a substring is invalid:
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
| `<<=` | right shift assign |
| `>>=` | left shift assign |
| `&=` | `and` assign |
| `\|=` | `or` shift assign |
| `^=` | `xor` shift assign |
| `<<` | left shift |
| `>>` | right shift |
| `&` | `and` |
| `\|` | `or` |
| `^` | `xor` |
| `~` | `not` |

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
| `set` | set bit(s)|
| `reset` | reset bits(s)|
| `flip` | flip bits(s)|
| `to_ulong` | convert to `unsigned long` |
| `to_ullong` | convert to `unsigned long long` |
| `count` | count of bits on |
| `size` | number of bits in bitset |
| `operator[]` | test bit at position |

Additional methods
| Method | Description |
| :--- | :--- |
| `set_all` | set all specified bits |
| `reset_all` | reset all specified bits |
| `test_any` | test for one or more bits |
| `test_all` | test for all specified bits |

All accessors and mutators work with enum values or integers as with operators. They also work with multiple values, either as template parameters or
as variadic arguments:
```c++
enum_bitset<numbers> eb;
eb.set_all<numbers::zero,numbers::two,numbers::five,numbers::nine>();
std::cout << eb << '\n';
std::cout << std::boolalpha << eb.test_all<numbers::zero,numbers::two,numbers::five,numbers::nine>() << '\n';
eb.reset<numbers::five,numbers::two>();
std::cout << std::boolalpha << eb.test_all(0, 2, 5, 9) << '\n';
std::cout << std::boolalpha << eb.test_any(0, 2, 5, 9) << '\n';
std::cout << std::boolalpha << eb.test_all(numbers::zero,numbers::nine) << '\n';
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
### iii. `for_each`
```c++
template<typename Fn, typename... Args>
requires std::invocable<Fn&&, T, Args...>
[[maybe_unused]] constexpr auto for_each(Fn&& func, Args&&... args);

template<typename C, typename Fn, typename... Args> // specialisation for member function with object
requires std::invocable<Fn&&, C, T, Args...>
[[maybe_unused]] constexpr auto for_each(Fn&& func, C *obj, Args&&... args);
```
Call supplied invocable for _each bit that is on_. Similar to `std::for_each` except first parameter of your invocable must accept an enum value (passed by `for_each`).
Optionally provide any additional parameters. Works with lambdas, member functions, functions etc. The second version is intended to be used
when using a member function - the _second_ parameter passed by your call must be the `this` pointer of the object.
If you wish to pass a `reference` parameter, you must wrap it in `std::ref`.

Returns `std::bind(std::forward<Fn>(func), std::placeholders::_1, std::forward<Args>(args)...)` or
`std::bind(std::forward<Fn>(func), obj, std::placeholders::_1, std::forward<Args>(args)...)` which can be stored or immediately invoked.

To iterate over each bit regardless of whether it is on or not, use `conjure_enum<T>::for_each`.

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

---
# 5. API and Examples using `conjure_type`
`conjure_type` is a general purpose class allowing you to extract a string representation of any type.
The string will be stored statically by the compiler, so use the statically generated value `name` for your type.
```c++
template<typename T>
class conjure_type;
static constexpr fixed_string name;
```
This static member is generated for your type. It is a `fixed_string` but has a built-in `std::string_view` operator.

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

---
# 6. Building
This implementation is header only. Apart from standard C++20 includes there are no external dependencies needed in your application.
[Catch2](https://github.com/catchorg/Catch2.git) is used for the built-in unit tests.

## a) Obtaining the source, building the examples
### \*nix based environments
To clone and default build the test app, unit tests and the benchmark:
```bash
git clone https://github.com/fix8mt/conjure_enum.git
cd conjure_enum
mkdir build
cd build
cmake ..
make -j4
make test (or ctest)
```
By default all warnings are enabled. To prevent this, pass the following to cmake:
```cmake
cmake -DBUILD_ALL_WARNINGS=false ..
```
By default the unit tests are built (which will download Catch2). To prevent this, pass the following to cmake:
```cmake
cmake -DBUILD_UNITTESTS=false ..
```
To disable certain C++20 features that cause issues with some builds:
```cmake
cmake -DBUILD_CONSRVCPP20=true ..
```
### Windows environments
Create a new console project. Add the repo `https://github.com/fix8mt/conjure_enum.git` and clone the source.
Make sure you set the C++ language to C++20 in the project preferences. The project should build and run the unit tests
by default.

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

---
# 7. Notes
## a) enum limits
### `ENUM_MIN_VALUE`, `ENUM_MAX_VALUE`
These are set by default unless you override them by defining them in your application.
> [!NOTE]
> If you want to define these values they must appear _before_ you include `conjure_enum.hpp`.

The following are the default settings:
```c++
#if not defined ENUM_MIN_VALUE
# define ENUM_MIN_VALUE -128
#endif
#if not defined ENUM_MAX_VALUE
# define ENUM_MAX_VALUE 127
#endif
```
These definitions set the minimum and maximum enum values that are supported. You can adjust them to suit your requirements but for most use cases the defaults are sufficient.

## b) Class `conjure_enum` is not constructible
All methods in this class are _static_. You cannot instantiate an object of this type.

## c) It's not _real_ reflection
This library provides a workaround to current limitations of C++. There are proposals out there for future versions of the language that will provide proper reflection.
See [Reflection TS](https://en.cppreference.com/w/cpp/experimental/reflect) and [Reflection for C++26](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2996r0.html)
for examples of some of these.

## d) Use of `std::string_view`
All of the generated static strings and generated static tables obtained by `std::source_location` use the library defined `fixed_string`. No string copying is done at runtime, resulting in
a single static string in your application. All `conjure_enum` methods that return strings _only_ return `std::string_view`.
To demonstrate this, lets look at the supplied test application `statictest`:
```c++
#include <iostream>
#include <fix8/conjure_enum.hpp>
enum class component : int { scheme, authority, userinfo, user, password, host, port, path, query, fragment };
int main(void)
{
   for(const auto& [a, b] : conjure_enum<component>::entries)
      std::cout << conjure_enum<component>::enum_to_int(a) << ' ' << b << '\n';
   for(const auto& [a, b] : conjure_enum<component>::unscoped_entries)
      std::cout << conjure_enum<component>::enum_to_int(a) << ' ' << b << '\n';
   for(const auto& a : conjure_enum<component>::names)
      std::cout << a << '\n';
   for(const auto& a : conjure_enum<component>::unscoped_names)
      std::cout << a << '\n';
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
1 authority
9 fragment
5 host
4 password
7 path
6 port
8 query
0 scheme
3 user
2 userinfo
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
$
```

The default build of `statictest` performs a [strip](https://en.wikipedia.org/wiki/Strip_(Unix)) on the executable.
Then we run [strings](https://en.wikipedia.org/wiki/Strings_(Unix)) on the executable.
<details><summary><i>shell output</i></summary>
<p>

```CSV
$ strings statictest
/lib64/ld-linux-x86-64.so.2
__gmon_start__
_ITM_deregisterTMCloneTable
_ITM_registerTMCloneTable
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
AVAUATUH
[]A\A]A^
PTE1
u+UH
component::fragment
component::query
component::path
component::port
component::host
component::password
component::user
component::userinfo
component::authority
component::scheme
9*3$"
GCC: (Ubuntu 14-20240412-0ubuntu1) 14.0.1 20240412 (experimental) [master r14-9935-g67e1433a94f]
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
```
</p>
</details>

It can be observed that there is only _one_ copy of the scoped enum value string in the executable.

---
# 8. Compiler support
| Compiler | Version(s) | Notes | Unsupported |
| :--- | :--- | :--- | ---: |
| [gcc](https://gcc.gnu.org/projects/cxx-status.html) | `11`, `12`, `13`, `14`| `std::format` not complete in `11`, `12` | `<= 10` |
| [clang](https://clang.llvm.org/cxx_status.html) | `15`, `16`, `17`, `18`| Catch2 needs `cxx_std_20` in `15` | `<= 14` |
| [msvc](https://learn.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance) | `16`, `17` | Visual Studio 2019,2022, latest `17.9.5`| `<= 16.9`|
| [xcode](https://developer.apple.com/support/xcode/) | `15` | Some issues with `constexpr`, workarounds| `<= 14`|

# 9. Compiler issues
| Compiler | Version(s) | Issues | Workaround |
| :--- | :--- | :--- | ---: |
| clang | `16`, `17`, `18`| Compiler reports integers outside valid range [x,y]| specify underlying type when declaring enum eg. `enum class foo : int` |

[^1]:&copy; 2024 Fix8 Market Technologies Pty Ltd, David L. Dight.
[^2]:&copy; 2019 - 2024 Daniil Goncharov
