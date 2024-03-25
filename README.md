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
- no macros
- does not use `__PRETTY_FUNCTION__` or `__FUNCTION__`
- class based
- support for scoped and unscoped enums 

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
