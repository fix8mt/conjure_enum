# conjure_enum

### Lightweight header-only C++20 enum reflection

------------------------------------------------------------------------
# Introduction
This is a lightweight enum reflection class based on [magic_enum](https://github.com/Neargye/magic_enum).

## Motivation
- header-only
- no external dependencies
- simplicity, lightweight
- make use of C++20 features
- entirely `constexpr`

## Features
- single _header-only_
- all methods `constexpr`; no virtual methods
- implements subset of most used features
- no macros, pure c++20
- does not use `__PRETTY_FUNCTION__` or `__FUNCTION__`
- class based
- support for scoped and unscoped enums
- support for enum aliases, gaps
- support for gcc, clang; msvc TBA

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
