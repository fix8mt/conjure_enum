<p align="center">
  <a href="https://www.fix8mt.com"><img src="https://github.com/fix8mt/conjure_enum/blob/master/assets/fix8mt_Master_Logo_Green_Trans.png" width="200"></a>
</p>

# conjure_enum

### Lightweight header-only C++20 enum reflection

------------------------------------------------------------------------
[![Ubuntu](https://github.com/fix8mt/conjure_enum/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/fix8mt/conjure_enum/actions/workflows/cmake-single-platform.yml)
<a href="https://en.wikipedia.org/wiki/C%2B%2B20"><img src="https://github.com/fix8mt/conjure_enum/blob/master/assets/badgecpprequired.svg"></a>
<a href="https://opensource.org/license/mit"><img src="https://github.com/fix8mt/conjure_enum/blob/master/assets/badgemitlic.svg"></a>

# Quick links
|**Link**|**Description**|
--|--
|[Here](https://github.com/fix8mt/conjure_enum/blob/master/include/fix8/conjure_enum.hpp)| For implementation|
|[Examples](#examples)| For examples|
|[Building](#building)| How to build or include|

------------------------------------------------------------------------
# Introduction
## Supercharge Your C++ Enums with This Lightweight Reflection Library!

Based on the awesome work of [magic_enum](https://github.com/Neargye/magic_enum), this library offers a streamlined and powerful
way to add reflection capabilities to your C++ enums. We've optimized the core functionality, focusing on the
features developers really need while enhancing them for a more efficient and expressive experience.

## Embrace the Future with C++20:

This library takes full advantage of the latest C++20 features. We've leveraged the convenience of `std::source_location` and
unlocked the potential of `constexpr` algorithms and concepts. This translates to:

- Improved Performance: Optimized code for faster and smoother operation - get the compiler to do more!
- Enhanced Developer Experience: Write cleaner, more concise, and more powerful C++ code.

## Key Benefits:

- Lightweight: Designed for performance without unnecessary overhead.
- Header-Only: No external dependencies, simplifying integration into your project.
- Modern C++20: Entirely constexpr for compile-time safety, efficiency and performance.
- Simple & Easy to Use: Class-based approach with intuitive syntax.
- Broad Support: Works with scoped and unscoped enums, enum aliases, and even with gaps.
- Wide Compiler Compatibility: Supports GCC, Clang, and upcoming MSVC support.
- Confidence in Quality: Includes built-in unit tests for reliable functionality.

## This library empowers you to:

- Gain deep insights into your enums at runtime.
- Unlock the power of metaprogramming for advanced code generation techniques.
- Write more expressive and efficient C++ code.

# Examples
All examples refer to the following enums:
```c++
enum class component : int { scheme, authority, userinfo, user, password, host, port, path=12, test=path, query, fragment };
enum component1 : int { scheme, authority, userinfo, user, password, host, port, path=12, query, fragment };
enum class numbers : int { zero, one, two, three, four, five, six, seven, eight, nine };
```

## `enum_to_string`
```c++
static constexpr std::string_view enum_to_string(T value, bool noscope=false);
```
Returns a `std::string_view` (empty if not found). Optionally pass `true` will remove scope in result if present.
```c++
auto name { conjure_enum<component>::enum_to_string(component::path) };
auto name_trim { conjure_enum<component>::enum_to_string(component::path, true) }; // optionally remove scope in result
auto alias_name { conjure_enum<component>::enum_to_string(component::test) }; // alias
auto noscope_name { conjure_enum<component1>::enum_to_string(path) };
auto invalid_name { conjure_enum<component>::enum_to_string(static_cast<component>(100)) };
std::cout << name << '\n' << name_trim << '\n' << alias_name << '\n' << noscope_name << '\n';
std::cout << std::format(R"("{}")", conjure_enum<component>::enum_to_string(static_cast<component>(100))) << '\n';
```
_output_
```CSV
component::path
path
component::path
path
""
```
## `get_name`
```c++
static constexpr std::string_view get_name();
```
Returns a `std::string_view` (empty if not found). This is the template version of `enum_to_string`.
```c++
std::cout << conjure_enum<component>::get_name<component::scheme>() << '\n';
std::cout << conjure_enum<component1>::get_name<scheme>() << '\n';
```
_output_
```CSV
component::scheme
scheme
```

## `string_to_enum`
```c++
static constexpr std::optional<T> string_to_enum(std::string_view str);
```
Returns a `std::optional<T>`. Empty if string was not valid.
```c++
int value { static_cast<int>(conjure_enum<component>::string_to_enum("component::path").value()) };
int value_trim { static_cast<int>(conjure_enum<component>::string_to_enum("path", true).value()) }; // optionally remove scope in test
int noscope_value { static_cast<int>(conjure_enum<component1>::string_to_enum("path").value()) };
int bad_value { static_cast<int>(conjure_enum<component>::string_to_enum("bad_string").value_or(component(100))) };
std::cout << value << '\n' << value_trim << '\n' << noscope_value << '\n' << bad_value << '\n';
```
_output_
```CSV
12
12
12
100
```
## `int_to_enum`
```c++
static constexpr std::optional<T> int_to_enum(int value);
```
Returns a `std::optional<T>`. Empty if value was not valid.
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
100
```
## `count`
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
## `enum_names`
```c++
static constexpr std::array<std::string_view, std::size_t> enum_names;
```
This static member is generated for your type. It is a `std::array` of the `std::string_view` strings.
```c++
for(const auto ev : conjure_enum<component>::enum_names) // scoped
   std::cout << ev << '\n';
for(const auto ev : conjure_enum<component1>::enum_names) // unscoped
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
## `enum_values`
```c++
static constexpr std::array<T, std::size_t> enum_values;
```
This static member is generated for your type. It is a `std::array` of the `T` values.
```c++
for(const auto ev : conjure_enum<component>::enum_values) // scoped
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
## `enum_entries`
```c++
static constexpr std::array<std::tuple<T, std::string_view>, std::size_t> enum_entries;
```
This static member is generated for your type. It is a `std::array` of tuples of `T` and `std::string_view`.
```c++
for(const auto [value, str] : conjure_enum<component>::enum_entries) // scoped
   std::cout << static_cast<int>(value) << ' ' str << '\n';
```
_output_
```CSV
0 component::scheme
1 component::authority
2 component::userinfo
3 component::user
4 component::password
5 component::host
6 component::port
12 component::path
13 component::query
14 component::fragment
```
## `enum_scoped_entries`
```c++
static constexpr std::array<std::tuple<std::string_view, std::string_view>, std::size_t> enum_scoped_entries;
```
This static member is generated for your type. It is a `std::array` of a tuple of `std::string_view` pairs.
It contains pairs of scoped and their unscoped string version. This array is sorted by unscoped name.
For unscoped enums, these are identical.
```c++
for(const auto [a, b] : conjure_enum<component>::enum_scoped_entries)
   std::cout << a << ' ' << b << '\n';
```
_output_
```CSV
authority component::authority
fragment component::fragment
host component::host
password component::password
path component::path
port component::port
query component::query
scheme component::scheme
user component::user
userinfo component::userinfo
```
## `for_each`
```c++
template<typename Fn, typename... Args>
requires std::invocable<Fn&&, T, Args...>
[[maybe_unused]] static constexpr auto for_each(Fn&& func, Args&&... args);
```
Call supplied invocable for each enum. Similar to `std::for_each` except first parameter of your invocable must accept an enum value (passed by `for_each`).
Optionally provide any additional parameters. Works with lambdas, member functions, functions etc. When using a member function, the _first_ parameter
passed by your call must be the `this` pointer of the object. If you wish to pass a `reference` parameter, you must wrap it in
`std::ref`.

Returns `std::bind(std::forward<Fn>(func), std::placeholders::_1, std::forward<Args>(args)...)` which can be stored or immediately invoked.
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
## `is_scoped`
```c++
struct is_scoped : std::integral_constant<bool, requires
   { requires !std::is_convertible_v<T, std::underlying_type_t<T>>; }>{};
```
Returns a `bool`
```c++
std::cout << std::boolalpha << conjure_enum<component>::is_scoped() << '\n';
std::cout << std::boolalpha << conjure_enum<component1>::is_scoped() << '\n';
```
_output_
```CSV
true
false
```
## `is_valid`
```c++
template<T e>
static constexpr bool is_valid();
```
Returns `true` if enum value is valid.
```c++
std::cout << std::boolalpha << conjure_enum<component>::is_valid<component::password>() << '\n';
std::cout << std::boolalpha << conjure_enum<component>::is_valid<static_cast<component>(16)>() << '\n';
```
_output_
```CSV
true
false
```
## `get_type`
```c++
static constexpr std::string_view get_type();
```
Returns a `std::string_view` of `T`.
```c++
std::cout << conjure_enum<component>::get_type() << '\n';
std::cout << conjure_enum<component1>::get_type() << '\n';
```
_output_
```CSV
component
component1
```
## `remove_scope`
```c++
static constexpr std::string_view remove_scope(std::string_view what);
```
Returns a `std::string_view` with scope removed; for unscoped returns unchanged
```c++
for(const auto ev : conjure_enum<component>::enum_names) // scoped
   std::cout << conjure_enum<component>::remove_scope(ev) << '\n';
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
```
## `add_scope`
```c++
static constexpr std::string_view add_scope(std::string_view what);
```
Returns a `std::string_view` with scope added to the enum if the supplied enum string is valid but missing scope; for unscoped returns unchanged
```c++
std::cout << conjure_enum::add_scope<component>("path"sv) << '\n';
```
_output_
```CSV
component::path
```
## `has_scope`
```c++
static constexpr bool has_scope(std::string_view what);
```
Returns a `true` if the supplied string is scoped (and is valid).
```c++
std::cout << std::boolalpha << conjure_enum<component>::has_scope("component::scheme") << '\n';
std::cout << std::boolalpha << conjure_enum<component>::has_scope("scheme") << '\n';
std::cout << std::boolalpha << conjure_enum<component1>::has_scope("scheme") << '\n';
```
_output_
```CSV
true
false
false
```
## `epeek, tpeek`
```c++
static consteval const char *tpeek();
template<T e>
static consteval const char *epeek();
```
These functions return the templated `std::source_location` `const char*` strings for the enum type or enum values.
The actual output is implementation dependent. When reporting an issue please include the output of these methods.
The following is the output with GCC 13.
```c++
std::cout << conjure_enum<component>::tpeek() << '\n';
std::cout << conjure_enum<component>::epeek<component::scheme>() << '\n';
```
_output_
```CSV
static consteval const char* FIX8::conjure_enum<T>::tpeek() [with T = component]
static consteval const char* FIX8::conjure_enum<T>::epeek() [with T e = component::scheme; T = component]
```
# Building
This implementation is header only. Apart from standard C++20 includes there are no external dependencies needed in your application.
[Catch2](https://github.com/catchorg/Catch2.git) is used for the built-in unit tests.

## Obtaining the source, building the examples
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
By default the unit tests are built (will download Catch2). To prevent this, pass the following to cmake:
```bash
cmake -DBUILD_UNITTESTS=false ..
```
## Using in your application with cmake
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

## Integrating `conjure_enum` in your project with cmake FetchContent
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
