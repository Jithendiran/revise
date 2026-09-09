## Concepts (C++20)

### Unconstrained
An unconstrained template is a traditional C++ template that places no explicit restrictions or requirements on its template arguments.  

```cpp
template <typename T>
T print_and_add(T a, T b) {
    return a + b;
}
```

The compiler accepts any type passed to the template. It does not verify whether the type supports operations performed inside the function body until the template is explicitly instantiated.
* Passing int works because integers support the addition operator.
* Passing a custom struct lacking operator+ compiles initially, but instantiating the template with that struct triggers a lengthy, complex compilation error deep inside the function body.

Advantages include maximum flexibility for generic code. Disadvantages include late error detection and cryptic compiler output when types do not support required operations.
### The Problem With Unconstrained Templates

When the wrong type is used with an unconstrained template, the error appears deep inside the template instantiation — far from the call site and difficult to read:

```cpp
template<typename T>
T max(T a, T b) { return a > b ? a : b; }

struct NoComparison {};
max(NoComparison{}, NoComparison{});
// error: "no match for operator>" — points inside the template body
// not at the call site — confusing
```

### What a Concept Is

A concept is a named compile-time predicate that **constrains** what types a template accepts. When violated, the error is clear and appears at the call site.

```cpp
#include <concepts>

template<typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
    { a > b } -> std::convertible_to<bool>;
};

template<Comparable T>
T max(T a, T b) { return a > b ? a : b; }

max(NoComparison{}, NoComparison{});
// error AT CALL SITE:
// "constraints not satisfied: NoComparison does not satisfy Comparable"
```
* `concept Comparable`: Declares a reusable constraint named Comparable.
* `requires(T a, T b)`: Creates a temporary evaluation context with two parameters, a and b, of type T.
* `{ a < b } -> std::convertible_to<bool>;`: Asserts that the expression a < b is valid code and that its result type can be converted to bool.
* `{ a > b } -> std::convertible_to<bool>;`: Asserts that the expression a > b is valid code and that its result type can be converted to bool.

It specifies that any type constrained by this definition must support less-than (`{ a < b }`) and greater-than (`{ a > b }`) comparisons, and the results of those comparisons must be convertible (`-> std::convertible_to<bool>`) to boolean values.

### Standard Library Concepts

```
Core language concepts (in <concepts>):
  std::same_as<T, U>          — T and U are the same type
  std::derived_from<T, Base>  — T derives from Base
  std::convertible_to<T, U>   — T converts to U
  std::integral<T>            — T is an integer type
  std::floating_point<T>      — T is floating-point
  std::signed_integral<T>     — T is a signed integer
  std::unsigned_integral<T>   — T is unsigned integer
  std::constructible_from<T, Args...>
  std::copy_constructible<T>
  std::move_constructible<T>
  std::assignable_from<T, U>
  std::swappable<T>
  std::destructible<T>
  std::default_initializable<T>

Comparison concepts:
  std::equality_comparable<T>   — T supports == and !=
  std::totally_ordered<T>       — T supports all six comparisons
  std::three_way_comparable<T>  — T supports <=>

Iterator/range concepts (in <iterator>, <ranges>):
  std::input_iterator<T>
  std::forward_iterator<T>
  std::bidirectional_iterator<T>
  std::random_access_iterator<T>
  std::range<T>       — T has begin() and end()
  std::sized_range<T> — T has size()

Callable concepts:
  std::invocable<F, Args...>    — F callable with Args
  std::predicate<F, Args...>    — F(Args) returns bool-convertible
```

### Four Syntax Forms — All Equivalent

```cpp
// Form 1: concept name in parameter list
template<std::integral T>
T add(T a, T b) { return a + b; }

// Form 2: requires clause after parameter list
template<typename T>
    requires std::integral<T>
T add(T a, T b) { return a + b; }

// Form 3: requires clause after function signature
template<typename T>
T add(T a, T b) requires std::integral<T> { return a + b; }

// Form 4: abbreviated function template
std::integral auto add(std::integral auto a, std::integral auto b) {
    return a + b;
}
```

### Defining Custom Concepts

```cpp
// Type must have value_type, begin(), and end()
template<typename T>
concept Container = requires(T c) {
    typename T::value_type;
    { c.begin() } -> std::input_iterator;
    { c.end()   } -> std::input_iterator;
};

// Type must be printable to ostream
template<typename T>
concept Printable = requires(T v, std::ostream& os) {
    { os << v } -> std::same_as<std::ostream&>;
};

// Compound concept
template<typename T>
concept PrintableContainer =
    Container<T> && Printable<typename T::value_type>;
```

### Concept Subsumption — More Constrained Overload Wins

```cpp
template<std::integral T>
void process(T v) { std::cout << "integral\n"; }

template<std::signed_integral T>   // signed_integral implies integral
void process(T v) { std::cout << "signed integral\n"; }

process(5);    // "signed integral" — more constrained wins
process(5u);   // "integral"        — unsigned satisfies integral only
```
