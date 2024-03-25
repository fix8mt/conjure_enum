<p align="center">
  <a href="https://www.fix8mt.com"><img src="https://github.com/fix8mt/conjure_enum/blob/master/assets/fix8mt_Master_Logo_Green_Trans.png" width="200"></a>
</p>

# conjure_enum

### Lightweight header-only C++20 enum reflection

------------------------------------------------------------------------
[![Ubuntu](https://github.com/fix8mt/conjure_enum/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/fix8mt/conjure_enum/actions/workflows/cmake-single-platform.yml)
<a href="https://en.wikipedia.org/wiki/C%2B%2B20"><img src="https://github.com/fix8mt/conjure_enum/blob/master/assets/badgecpprequired.svg"></a>
<a href="https://http://www.boost.org/LICENSE_1_0.txt"><img src="https://github.com/fix8mt/conjure_enum/blob/master/assets/badgeboostlic.svg"></a>

# Quick links
|**Link**|**Description**|
--|--
|[Here](https://github.com/fix8mt/conjure_enum/blob/master/include/fix8/conjure_enum.hpp)| For implementation|
|[Examples](#examples)| For examples|
|[Building](#building)| How to build or include|

> [!TIP]
> Use the built-in [table of contents](https://github.blog/changelog/2021-04-13-table-of-contents-support-in-markdown-files/) to navigate this guide.

------------------------------------------------------------------------
# Introduction
This is a lightweight enum reflection class based on [magic_enum](https://github.com/Neargye/magic_enum).
We reworked the core of the library, stripped back all but the most basic functionality that most
developers look for. We updated the code to C++20, taking advantage of `std::source_location`, as well as other
improvements only available in C++20 (such as `constexpr` algorthims, `consteval`, concepts, `to_array` and so forth).

## Motivation
- header-only
- no external dependencies
- simplicity, lightweight
- make use of C++20 features
- entirely `constexpr`

## Features
- single _header-only_
- all methods `constexpr`; no virtual methods
- implements subset of the most used features
- no macros (`__PRETTY_FUNCTION__`, `__FUNCTION__` or `__FUNCSIG__`), pure c++20
- class based
- support for scoped and unscoped enums
- support for enum aliases, gaps
- support for GCC, Clang; MSVC (TBA)
- built-in unit tests

# Examples
```c++
enum class component : int { scheme, authority, userinfo, user, password, host, port, path=12, test=path, query, fragment };
enum component1 : int { scheme, authority, userinfo, user, password, host, port, path=12, query, fragment };
```

### `enum_to_string`
```c++
auto name { conjure_enum::enum_to_string(component::path) };
auto alias_name { conjure_enum::enum_to_string(component::test) }; // alias
auto noscope_name { conjure_enum::enum_to_string(path) };
std::cout << name << '\n' << alias_name << '\n' << noscope_name << '\n';
```
_output_
```CSV
component::path
component::path
path
```

### `string_to_enum`
```c++
int value { static_cast<int>(conjure_enum::string_to_enum<component>("component::path").value()) };
int noscope_value { static_cast<int>(conjure_enum::string_to_enum<component1>("path").value()) };
int bad_value { static_cast<int>(conjure_enum::string_to_enum<component>("bad_string").value_or(component(100))) };
std::cout << value << '\n' << noscope_value << '\n' << bad_value << '\n';
```
_output_
```CSV
12
12
100
```
### `int_to_enum`
```c++
int value { static_cast<int>(conjure_enum::int_to_enum<component>(12).value()) };
int noscope_value { static_cast<int>(conjure_enum::int_to_enum<component1>(12).value()) };
int bad_value { static_cast<int>(conjure_enum::int_to_enum<component>(100).value_or(component(100))) };
std::cout << value << '\n' << noscope_value << '\n' << bad_value << '\n';
```
_output_
```CSV
12
12
100
```
### `count`
```c++
std::cout << conjure_enum::count<component>()  << '\n';
```
_output_
```CSV
10
```
### `enum_names`
```c++
for(const auto ev : conjure_enum::enum_names<component>) // scoped
   std::cout << ev << '\n';
for(const auto ev : conjure_enum::enum_names<component1>) // unscoped
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
component::host
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
host
```
### `enum_values`
```c++
for(const auto ev : conjure_enum::enum_values<component>) // scoped
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
### `enum_entries`
```c++
for(const auto [value, str] : conjure_enum::enum_entries<component>) // scoped
   std::cout << value << ' ' str << '\n';
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
### `is_scoped`
```c++
std::cout << std::boolalpha << conjure_enum::is_scoped<component>() << '\n';
std::cout << std::boolalpha << conjure_enum::is_scoped<component1>() << '\n';
```
_output_
```CSV
true
false
```
### `is_valid`
```c++
std::cout << std::boolalpha << conjure_enum::is_valid<component, component::password>() << '\n';
std::cout << std::boolalpha << conjure_enum::is_valid<component, static_cast<component>(16)>() << '\n';
```
_output_
```CSV
true
false
```
### `get_type`
```c++
std::cout << conjure_enum::get_type<component>() << '\n';
std::cout << conjure_enum::get_type<component1>() << '\n';
```
_output_
```CSV
component
component1
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
