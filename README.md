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
|[Examples](#examples)| General examples|
|[Examples with enum_bitset](#examples-using-enum_bitset)| `enum_bitset` examples|
|[Building](#building)| How to build or include|
|[Notes](#notes)| Notes on the implementation, restrictions and caveats|

------------------------------------------------------------------------
# Introduction
## Supercharge Your C++ Enums with This Lightweight Reflection Library!

Based on the awesome work of [`magic_enum`](https://github.com/Neargye/magic_enum)[^2], this library offers a streamlined and powerful
way to add reflection capabilities to your C++ enums. We've optimized the core functionality, focusing on the
features developers really need while enhancing them for a more efficient and expressive experience.

## Embrace the Future with C++20

`conjure_enum`[^1] takes full advantage of the latest C++20 features. We've leveraged the convenience of `std::source_location` and
unlocked the potential of `constexpr` algorithms and concepts. This translates to:

- Improved Performance: Optimized code for faster and smoother operation - get the compiler to do more!
- Enhanced Developer Experience: Write cleaner, more concise, and more powerful C++ code.

## Key Benefits

- Lightweight: Designed for performance without unnecessary overhead.
- Header-Only: No external dependencies, simplifying integration into your project.
- Modern C++20: Entirely `constexpr` for compile-time safety, efficiency and performance.
- Simple & Easy to Use: Class-based approach with intuitive syntax.
- Convenient: `enum_bitsets` enhance `std::bitset`.
- Broad Support: Works with scoped and unscoped enums, enum aliases and even with gaps.
- Wide Compiler Compatibility: Supports GCC, Clang, and upcoming MSVC support.
- Confidence in Quality: Includes built-in unit tests for reliable functionality.

## This library empowers you to

- Gain deeper insights into your enums at runtime.
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
Returns a `std::string_view` or empty if not found. Optionally passing `true` will remove scope in result if present.
```c++
auto name { conjure_enum<component>::enum_to_string(component::path) };
auto name_trim { conjure_enum<component>::enum_to_string(component::path, true) }; // optionally remove scope in result
auto alias_name { conjure_enum<component>::enum_to_string(component::test) }; // alias
auto noscope_name { conjure_enum<component1>::enum_to_string(path) };
std::cout << name << '\n' << name_trim << '\n' << alias_name << '\n' << noscope_name << '\n';
```
### Aliases
Because all methods in `conjure_enum` are within a `class` instead of individual template functions in a `namespace`, you can reduce your
typing with standard aliases:
```c++
using ec = conjure_enum<component>;
std::cout << std::format("\"{}\"\n", ec::enum_to_string(static_cast<component>(100)));
```
_output_
```CSV
component::path
path
component::path
path
""
```
Also supplied is a template version of `enum_to_string`.
```c++
std::cout << std::format("\"{}\"\n", conjure_enum<component>::enum_to_string<component::scheme>());
std::cout << std::format("\"{}\"\n", conjure_enum<component1>::enum_to_string<scheme>());
```
_output_
```CSV
"component::scheme"
"scheme"
```

## `string_to_enum`
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
## `int_to_enum`
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
```
## `enum_scoped_entries`
```c++
static constexpr std::array<std::tuple<std::string_view, std::string_view>, std::size_t> enum_scoped_entries;
```
This static member is generated for your type. It is a `std::array` of a tuple of `std::string_view` pairs.
It contains pairs of unscoped and their scoped string version. This array is sorted by unscoped name.
For unscoped enums, these are identical.
```c++
for(const auto [a, b] : conjure_enum<component>::enum_scoped_entries)
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
std::cout << std::format("{}\n", conjure_enum<component>::is_scoped());
std::cout << std::format("{}\n", conjure_enum<component1>::is_scoped());
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
std::cout << std::format("{}\n", conjure_enum<component>::is_valid<component::password>());
std::cout << std::format("{}\n", conjure_enum<component1>::is_valid<static_cast<component>(16)>());
```
_output_
```CSV
true
false
```
## `enum_type`
```c++
static constexpr std::string_view enum_type();
```
Returns a `std::string_view` of `T`.
```c++
std::cout << conjure_enum<component>::enum_type() << '\n';
std::cout << conjure_enum<component1>::enum_type() << '\n';
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
std::cout << conjure_enum::remove_scope<component>("component::path"sv) << '\n';
std::cout << conjure_enum::remove_scope<component1>("path"sv) << '\n';
```
_output_
```CSV
path
path
```
## `add_scope`
```c++
static constexpr std::string_view add_scope(std::string_view what);
```
Returns a `std::string_view` with scope added to the enum if the supplied enum string is valid but missing scope; for unscoped returns unchanged
```c++
std::cout << conjure_enum::add_scope<component>("path"sv) << '\n';
std::cout << conjure_enum::add_scope<component1>("path"sv) << '\n';
```
_output_
```CSV
component::path
path
```
## `has_scope`
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
## `epeek, tpeek`
```c++
static consteval const char *tpeek();
template<T e>
static consteval const char *epeek();
```
These functions return the `std::source_location` `const char*` strings for the enum type or enum value.
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
# Examples using `enum_bitset`
`enum_bitset` is a convenient and useful way of creating bitsets and is based on `std::bitset` except it uses your enum (scoped or unscoped)
for the bit positions (and names).
> [!NOTE]
> Your enum must start at `0` and be continuous. The last value must be less than the count of enumerations.

We decided on this restriction for both simplicity and practicality - bitsets only really make sense when represented in this manner.

## Creating an `enum_bitset`
You can use the enum values directly. No need to `|` them - this is assumed. Just supply them comma seperated:
```c++
enum_bitset<numbers> b(numbers::zero, numbers::one, numbers::two, numbers::three);
std::cout << b << '\n';
```
_output_
```CSV
0000001111
```
You can use the underlying types as well:
```c++
enum_bitset<numbers> b(0,1,2,3);
std::cout << b << '\n';
```
_output_
```CSV
0000001111
```
You can use an int initialiser too:
```c++
enum_bitset<numbers> b(15);
std::cout << b << '\n';
```
_output_
```CSV
0000001111
```
You can even use a delimited string based on your enum names.
Optionally omit the scope and even specify your own delimiter:
Substrings are trimmed of whitespace before lookup.
```c++
enum_bitset<numbers> b("numbers::zero,numbers::one,numbers::two,numbers::three");
std::cout << b << '\n';
enum_bitset<numbers> b1("zero,one  ,two,  three", true);
std::cout << b1 << '\n';
enum_bitset<numbers> b2("zero|one|two|three", true, '|');
std::cout << b2 << '\n';
```
_output_
```CSV
0000001111
0000001111
0000001111
```
A typical use of above is for parsing configuration bitsets. Here you can tell the contructor to throw an exception if a substring
is invalid:
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
## Standard bit operators
All of the standard operators are supported. Assignment operators return a `enum_bitset&`, non-assigment operators return a `enum_bitset`.

| Operator | Description |
| --- | --- |
| `<<=` | right shift assign |
| `>>=` | left shift assign |
| `&=` | and assign |
| `\|=` | or shift assign |
| `^=` | xor shift assign |
| `<<` | left shift |
| `>>` | right shift |
| `&` | and |
| `\|` | or |
| `^` | xor |
| `~` | not |

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
## Standard accessors and mutators
All of the standard accessors and mutators are supported.

| Method | Description |
| --- | --- |
| `test` | test for bit(s)|
| `set` | set bit(s)|
| `clear` | clear bits(s)|
| `flip` | flip bits(s)|
| `to_ulong` | convert to `unsigned long` |
| `to_ullong` | convert to `unsigned long long` |
| `count` | count of bits on |
| `size` | number of bits in bitset |
| `operator[]` | test bit at position |
| `test_any` | test for one or more bits |
| `test_all` | test for all specified bits |

All accessors and mutators work with enum values or integers as with operators. They also work with multiple values, either as template paramters or
as variadic arguments:
```c++
enum_bitset<numbers> eb;
eb.set_all<numbers::zero,numbers::two,numbers::five,numbers::nine>();
std::cout << eb << '\n';
std::cout << std::boolalpha << eb.test_all<numbers::zero,numbers::two,numbers::five,numbers::nine>() << '\n';
eb.clear_all<numbers::five,numbers::two>();
std::cout << std::boolalpha << eb.test_all(0, 2, 5, 9) << '\n';
std::cout << std::boolalpha << eb.test_any(0, 2, 5, 9) << '\n';
std::cout << std::boolalpha << eb.test_all(numbers::zero,numbers::nine) << '\n';
std::cout << eb << '\n';
eb.clear(numbers::nine)
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
## Other functions
# `std::ostream& operator<<`, `to_string`
```c++
friend constexpr std::ostream& operator<<(std::ostream& os, const enum_bitset& what);
constexpr std::string to_string(char zero='0', char one='1') const noexcept
```
Inserts default string representation into `std::ostream`.
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

# Notes
## Use of `std::string_view`
All of the generated static strings and generated static tables obtained by `std::source_location` use `std::string_view`. No string copying is done, resulting in
a single static string in your application. To demonstrate this, the default build of `example` performs a [strip](https://en.wikipedia.org/wiki/Strip_(Unix)) on the executable.

<details><summary><i>output</i></summary>
<p>

```CSV
$ strings example
/lib64/ld-linux-x86-64.so.2
m.w(
__gmon_start__
_ITM_deregisterTMCloneTable
_ITM_registerTMCloneTable
_ZNKSt6locale4nameB5cxx11Ev
_ZNSt6localeD1Ev
_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6insertEmmc
_ZSt20__throw_length_errorPKc
_ZNSo9_M_insertIbEERSoT_
_ZNSt6locale7classicEv
_ZSt9terminatev
_ZNSt13runtime_errorD2Ev
_ZSt8to_charsPcS_fSt12chars_format
_ZNKSt6locale2id5_M_idEv
_ZNSolsEi
_ZSt8to_charsPcS_dSt12chars_formati
_ZSt8to_charsPcS_dSt12chars_format
_ZNSt6localeC1ERKS_
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE7reserveEm
_ZNKSt13runtime_error4whatEv
_ZSt8to_charsPcS_eSt12chars_formati
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE15_M_replace_coldEPcmPKcmm
_ZNSt16invalid_argumentD1Ev
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_replaceEmmPKcm
_ZSt8to_charsPcS_d
_ZSt8to_charsPcS_e
_ZSt8to_charsPcS_f
_ZSt8to_charsPcS_fSt12chars_formati
_ZNSt16invalid_argumentC1EPKc
_ZNSt6localeC1Ev
_ZNKSt6localeeqERKS_
__cxa_throw
_ZTVN10__cxxabiv117__class_type_infoE
_ZSt9use_facetINSt7__cxx118numpunctIcEEERKT_RKSt6locale
_ZSt17__throw_bad_allocv
_ZTVN10__cxxabiv120__si_class_type_infoE
__cxa_begin_catch
_ZTISt13runtime_error
_ZSt4cerr
_ZdlPv
_ZNSt13runtime_errorC2EPKc
_ZSt16__throw_bad_castv
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEaSEOS4_
_ZNSt6localeaSERKS_
__cxa_allocate_exception
_ZSt21ios_base_library_initv
__gxx_personality_v0
_ZNSt7__cxx118numpunctIcE2idE
_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
_ZNSo3putEc
__cxa_demangle
_ZTISt16invalid_argument
_ZSt8to_charsPcS_eSt12chars_format
_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
_Znwm
_ZSt24__throw_out_of_range_fmtPKcz
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_mutateEmmPKcm
_ZSt4cout
__cxa_end_catch
_ZSt19__throw_logic_errorPKc
__cxa_free_exception
frexp
frexpf
frexpl
_Unwind_Resume
__udivti3
__cxa_finalize
memmove
__libc_start_main
memset
memchr
free
toupper
strlen
memcmp
memcpy
libstdc++.so.6
libm.so.6
libgcc_s.so.1
libc.so.6
GLIBC_2.14
GLIBC_2.34
GLIBC_2.2.5
GCC_3.0
GLIBCXX_3.4.32
GLIBCXX_3.4.20
GLIBCXX_3.4.31
GLIBCXX_3.4.9
GLIBCXX_3.4.21
CXXABI_1.3
GLIBCXX_3.4.29
GLIBCXX_3.4
T$(I9
L$(H9
D$(I9
D$ H
t$(H
L$PH9
H9l$PH
l$8u[H
\$ L
T$(H9
L$(I9
t$ H
\$ H
H;D$HH
L$(H9
D$(I9
L4#H
D$#L
L$#L)
[A\A]A^A_]
	w=E
wdfD
:t	@
0123456789abcdef
component::path
component::scheme
static const char *FIX8::conjure_enum<component>::epeek() [T = component, e = component::path]
static const char *FIX8::conjure_enum<component>::tpeek() [T = component]
"{}"
{:9} {}
{:<2} {}
"{}"
numbers::zero |numbers::two      |numbers::five| numbers::nine
zero|two|five|	nine
twenty,two,rubbish,nine
exception:
one|three|four|eight
one 		three four eight
basic_string::_M_replace_aux
basic_string::_M_create
basic_string_view::substr
%s: __pos (which is %zu) > __size (which is %zu)
static const char *FIX8::conjure_enum<component>::epeek() [T = component, e = component::scheme]
static const char *FIX8::conjure_enum<component1>::epeek() [T = component1, e = scheme]
basic_string: construction from null is not valid
memory allocation failiure
invalid mangled name
invalid argument
basic_string::append
basic_string_view::copy
format error: invalid arg-id in format string
format error: format-spec contains invalid formatting options for 'bool'
format error: width must be non-zero in format string
format error: invalid width or precision in format-spec
format error: unmatched '{' in format string
format error: conflicting indexing style in format string
format error: failed to parse format-spec
true
false
format error: integer not representable as character
format error: argument used for width or precision must be a non-negative integer
basic_string::_M_replace
format error: format-spec contains invalid formatting options for 'charT'
format error: missing precision after '.' in format string
basic_string::insert
%s: __pos (which is %zu) > this->size() (which is %zu)
format error: unmatched '}' in format string
component::authority
component::fragment
component::host
component::password
component::path
component::port
component::query
component::scheme
component::user
component::userinfo
numbers1::two
numbers1::one
numbers1::zero
numbers1::five
numbers1::six
numbers1::seven
numbers1::eight
numbers1::nine
numbers::eight
numbers::five
numbers::four
numbers::nine
numbers::one
numbers::seven
numbers::six
numbers::three
numbers::two
numbers::zero
St5arrayISt5tupleIJSt17basic_string_viewIcSt11char_traitsIcEES4_EELm10EE
NSt8__format9_Seq_sinkINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEEE
NSt8__format9_Buf_sinkIcEE
NSt8__format5_SinkIcEE
NSt8__format10_Iter_sinkIcNS_10_Sink_iterIcEEEE
NSt8__format19_Formatting_scannerINS_10_Sink_iterIcEEcEE
NSt8__format8_ScannerIcEE
St12format_error
 !"#
 !"#
00010203040506070809101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899
;*3$"
zPLR
GCC: (Ubuntu 13.2.0-4ubuntu3) 13.2.0
Ubuntu clang version 16.0.6 (15)
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
.text
.fini
.rodata
.eh_frame_hdr
.eh_frame
.gcc_except_table
.init_array
.fini_array
.data.rel.ro
.dynamic
.got.plt
.data
.bss
.comment
```
</p>
</details>
It can be observed that there is only one copy of the scoped or unscoped enum value string in the executable.

[^1]:&copy; 2024 Fix8 Market Technologies Pty Ltd, David L. Dight.
[^2]:&copy; 2019 - 2024 Daniil Goncharov
