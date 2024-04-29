<p align="center">
  <a href="https://www.fix8mt.com"><img src="https://github.com/fix8mt/conjure_enum/blob/master/assets/fix8mt_Master_Logo_Green_Trans.png" width="200"></a>
</p>

# `conjure_enum`

### Lightweight header-only C++20 enum reflection

------------------------------------------------------------------------
[![clang](https://github.com/fix8mt/conjure_enum/actions/workflows/Ubuntu-clang-latest.yml/badge.svg)](https://github.com/fix8mt/conjure_enum/actions/workflows/Ubuntu-clang-latest.yml)
[![gcc](https://github.com/fix8mt/conjure_enum/actions/workflows/Ubuntu-gcc-latest.yml/badge.svg)](https://github.com/fix8mt/conjure_enum/actions/workflows/Ubuntu-gcc-latest.yml)
<a href="https://en.wikipedia.org/wiki/C%2B%2B20"><img src="https://github.com/fix8mt/conjure_enum/blob/master/assets/badgecpprequired.svg"></a>
<a href="https://opensource.org/license/mit"><img src="https://github.com/fix8mt/conjure_enum/blob/master/assets/badgemitlic.svg"></a>

# Quick links
|**Link**|**Description**|
--|--
|[Here](https://github.com/fix8mt/conjure_enum/blob/master/include/fix8/conjure_enum.hpp)| For implementation|
|[API and Examples](#api-and-examples)| General examples|
|[API and Examples with `enum_bitset`](#api-and-examples-using-enum_bitset)| `std::bitset` replacement examples|
|[API and Examples with `conjure_type`](#api-and-examples-using-conjure_type)| any type string extractor examples|
|[Building](#building)| How to build or include|
|[Notes](#notes)| Notes on the implementation, limits, etc|
|[Compilers](#compiler-support)| Supported compilers|
|[Compiler issues](#compiler-issues)| Workarounds for various compiler issues|
> [!TIP]
> Use the built-in [table of contents](https://github.blog/changelog/2021-04-13-table-of-contents-support-in-markdown-files/) to navigate this guide.

------------------------------------------------------------------------
# Introduction
## Supercharge Your C++ Enums with This Lightweight Reflection Library!

Based on the awesome work of [`magic_enum`](https://github.com/Neargye/magic_enum)[^2] and [`boost::desribe`](https://github.com/boostorg/describe),
this library offers a streamlined and powerful way to add reflection capabilities to your C++ enums and other types. We've optimized the core functionality,
focusing on the main features developers usually need while enhancing and expanding them for a more efficient and expressive experience.

## Embrace the Future with C++20

`conjure_enum`[^1] takes full advantage of the latest C++20 features. We've leveraged the convenience of `std::source_location` and
unlocked the potential of `constexpr` algorithms and concepts. This translates to:

- ***Improved Performance***  Optimized code for faster and smoother operation - get the compiler to do more!
- ***Enhanced Developer Experience***  Write cleaner, more concise, and more powerful C++ code.

## Key Benefits

- ***Lightweight***:  Designed for performance without unnecessary overhead.
- ***Header-Only***:  No external dependencies, simplifying integration into your project.
- ***Modern C++20***:  Entirely `constexpr` for compile-time safety, efficiency and performance.
- ***Simple & Easy to Use***:  Class-based approach with intuitive syntax.
- ***Convenient***:  `enum_bitsets` offers an enhanced `std::bitset`.
- ***Useful***:  `conjure_type` lets you obtain the type string of _any type!_
- ***Broad Support***:  Works with scoped and unscoped enums, enum aliases and even with gaps.
- ***Wide Compiler Compatibility***:  Supports GCC, Clang, MSVC and XCode/Clang; `x86_64`, `AArch64`
- ***Confidence in Quality***:  Includes built-in unit tests for reliable functionality.

# API and Examples
All examples refer to the following enums:
```c++
enum class component { scheme, authority, userinfo, user, password, host, port, path=12, test=path, query, fragment };
enum component1 { scheme, authority, userinfo, user, password, host, port, path=12, query, fragment };
enum class numbers { zero, one, two, three, four, five, six, seven, eight, nine };
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
std::cout << std::format("\"{}\"\n", ec::enum_to_string(static_cast<component>(100)));
```
_output_
```CSV
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
## `names`
```c++
static constexpr std::array<std::string_view, std::size_t> names;
```
This static member is generated for your type. It is a `std::array` of the `std::string_view` strings in enum order.
```c++
for(const auto ev : conjure_enum<component>::names) // scoped
   std::cout << ev << '\n';
for(const auto ev : conjure_enum<component1>::names) // unscoped
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
## `values`
```c++
static constexpr std::array<T, std::size_t> values;
```
This static member is generated for your type. It is a `std::array` of the `T` values in enum order.
```c++
for(const auto ev : conjure_enum<component>::values) // scoped
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
## `entries`, `sorted_entries`
```c++
static constexpr std::array<std::tuple<T, std::string_view>, std::size_t> entries;
static constexpr std::array<std::tuple<T, std::string_view>, std::size_t> sorted_entries;
```
This static member is generated for your type. It is a `std::array` of tuples of `T` and `std::string_view`.
`sorted_entries` is the same as `entries` except the array is sorted by the `std::string_view` name.
```c++
using ec = conjure_enum<component>;
for(const auto [value, str] : ec::entries) // scoped
   std::cout << std::format("{:<2} {}\n", static_cast<int>(value), str);
std::cout << '\n';
for(const auto [value, str] : ec::sorted_entries) // scoped
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
## `scoped_entries`
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
## `contains`
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
## `for_each`
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
## `is_scoped`
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
## `type_name`
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
## `remove_scope`
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
## `add_scope`
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
## `iterators`
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
## `iterator_adaptor`
```c++
template<typename E, typename T=std::decay_t<E>>
requires std::is_enum_v<T>
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
## `front, back`
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
## `epeek, tpeek`
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
# API and Examples using `enum_bitset`
`enum_bitset` is a convenient way of creating bitsets based on `std::bitset`. It uses your enum (scoped or unscoped)
for the bit positions (and names).
> [!NOTE]
> Your enum should be continuous. The last value must be less than the count of enumerations.

We decided on this restriction for both simplicity and practicality - bitsets only really make sense when represented in this manner.

## Creating an `enum_bitset`
```c++
constexpr enum_bitset() = default;
constexpr enum_bitset(U bits);
constexpr enum_bitset(std::string_view from, bool anyscope=false,
   char sep='|', bool ignore_errors=true);

template<typename... E>
requires(std::is_enum_v<E> && ...)
constexpr enum_bitset(E... comp);

template<typename... I>
requires(std::is_integral_v<I> && ...)
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
## Standard bit operators
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
## Standard accessors and mutators
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
# Other functions
## `operator bool`
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

## `std::ostream& operator<<`, `to_string`
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
## `for_each`
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

# API and Examples using `conjure_type`
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
```
_output_
```CSV
std::map<long unsigned int, std::basic_string_view<char> >
std::map<long unsigned int, foo>
```
Works with its own types too:
```c++
std::cout << conjure_type<conjure_type<conjure_enum<numbers&>>>::name << '\n';
```
_output_
```CSV
conjure_type<conjure_enum<numbers&, numbers> >
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

# Building
This implementation is header only. Apart from standard C++20 includes there are no external dependencies needed in your application.
[Catch2](https://github.com/catchorg/Catch2.git) is used for the built-in unit tests.

## Obtaining the source, building the examples
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
## enum limits
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

## class `conjure_enum` is not constructible
All methods in this class are _static_. You cannot instantiate an object of this type.

## Use of `std::string_view`
All of the generated static strings and generated static tables obtained by `std::source_location` use the library defined `fixed_string`. No string copying is done at runtime, resulting in
a single static string in your application. All `conjure_enum` methods that return strings _only_ return `std::string_view`.
To demonstrate this, the default build of `example` performs a [strip](https://en.wikipedia.org/wiki/Strip_(Unix)) on the executable. Then we run
[strings](https://en.wikipedia.org/wiki/Strings_(Unix)) on the executable.

<details><summary><i>shell output</i></summary>
<p>

```CSV
$ ./example|strings
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
14 10
1000100101
true
false
true
true
1000000001
0000000001
0001001010
---+--+-+-
0001001010
0001001000
0001001010
1110110101
0001001010
true
authority 1
fragment 14
host 5
password 4
path 12
port 6
query 13
scheme 0
user 3
userinfo 2
numbers1::two1 2
numbers1::one1 3
numbers1::zero1 4
numbers1::five1 5
numbers1::six1 6
numbers1::seven1 7
numbers1::eight1 8
numbers1::nine1 9
component::path
component::path
path
component::scheme
scheme
true
false
false
static consteval const char* FIX8::conjure_enum<T>::epeek() [with T e = component::path; T = component]
static consteval const char* FIX8::conjure_enum<T>::tpeek() [with T = component]
static consteval const char* FIX8::conjure_enum<T>::epeek() [with T e = (component)100; T = component]
std::array<std::tuple<std::basic_string_view<char, std::char_traits<char> >, std::basic_string_view<char, std::char_traits<char> > >, 10ul>
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
*******************************
0000001111
0000001111
0000001111
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
true
"component::scheme"
1000100101
numbers::zero
numbers::two
numbers::five
numbers::nine
1000100101
numbers::zero
numbers::two
numbers::five
numbers::nine
0100011010
numbers::one
numbers::three
numbers::four
numbers::eight
0000001111
0000001111
0000000111
0000001011
0100011010
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
0 numbers::zero
9 numbers::nine
numbers::zero 10
numbers::one 10
numbers::three 10
numbers::zero 10
numbers::one 10
numbers::three 10
'numbers'
'FIX8::conjure_enum<numbers>'
'std::map<long unsigned int, std::basic_string_view<char> >'
'std::map<long unsigned int, main()::foo>'
'FIX8::conjure_type<FIX8::conjure_enum<numbers> >'
FIX8::fixed_string<58>
$
```
</p>
</details>
It can be observed that there is only one copy of the scoped or unscoped enum value string in the executable.

# Compiler support
| Compiler | Version(s) | Notes | Unsupported |
| :--- | :--- | :--- | ---: |
| [gcc](https://gcc.gnu.org/projects/cxx-status.html) | `11`, `12`, `13`| `std::format` not complete in `11`, `12` | `<= 10` |
| [clang](https://clang.llvm.org/cxx_status.html) | `15`, `16`| Catch2 needs `cxx_std_20` in `15` | `<= 14` |
| [msvc](https://learn.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance) | `16`, `17` | Visual Studio 2019,2022, latest `17.9.5`| `<= 16.9`|
| [xcode](https://developer.apple.com/support/xcode/) | `15` | Some issues with `constexpr`, workarounds| `<= 14`|

# Compiler issues
| Compiler | Version(s) | Issues | Workaround |
| :--- | :--- | :--- | ---: |
| clang | `16`| Compiler reports integers outside valid range [x,y]| specify underlying type when declaring enum eg. `enum class foo : int` |

[^1]:&copy; 2024 Fix8 Market Technologies Pty Ltd, David L. Dight.
[^2]:&copy; 2019 - 2024 Daniil Goncharov
