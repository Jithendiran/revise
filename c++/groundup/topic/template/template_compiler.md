## Template Instantiation — How the Compiler Generates Code

### Implicit Instantiation

When a template is used, the compiler automatically generates code:

```cpp
template<typename T>
T square(T x) { return x * x; }

square(5);    // generates: int    square(int    x) { return x * x; }
square(2.5);  // generates: double square(double x) { return x * x; }
```

### Explicit Instantiation

Forces code generation in one translation unit — reduces compile times and binary size:

```cpp
// In a .cpp file — generate here
template int    square<int>(int);
template double square<double>(double);

// In a header — tell other TUs not to instantiate
extern template int    square<int>(int);
extern template double square<double>(double);
```

### Only Used Members Are Instantiated

For class templates, only called member functions are compiled:

```cpp
template<typename T>
class Box {
    T value;
public:
    Box(T v) : value(v) {}

    void impossible() {
        value.nonExistentMethod();   // error in body
    }
};

Box<int> b(42);   // VALID: impossible() never called — never instantiated
b.impossible();   // NOW compile error: function instantiated here
```

### Templates Must Be Visible at Point of Use

Unlike regular functions, template definitions must be in headers because the compiler needs the full definition to instantiate:

* The requirement comes down to how the C++ compilation model works. Unlike regular functions, templates are not actual code; they are blueprints.
* The compiler doesn't generate machine code for a template when it reads the template definition. It waits until it sees an instantiation (like `Box<int> myBox;`).
* With regular functions, if `main.cpp` just sees a declaration (`void foo();`), it assumes the function exists somewhere else, compiles anyway, and leaves a placeholder for the linker to connect later. Templates don't work this way. The linker cannot instantiate templates; only the compiler can, and the compiler needs the full source code to do it not the blueprint code.

```cpp
// CORRECT: full definition in header
// box.h
template<typename T>
class Box { /* full definition */ };

// WRONG for templates: definition hidden in .cpp
// box.h
template<typename T>
class Box;   // declaration only — other .cpp files cannot instantiate
```

## SFINAE — Substitution Failure Is Not An Error

### What It Is

When the compiler substitutes deduced types into a template and the substitution produces an invalid type or expression, the compiler does not report an error — it silently discards that template from the candidate set and looks for other overloads.

### Why It Exists

```cpp
template<typename T>
typename T::value_type front(T container) {
    return container.front();
}

front(std::vector<int>{1, 2, 3});
// T = vector<int>, T::value_type = int — valid

front(42);
// T = int, int::value_type does not exist — invalid substitution
// WITHOUT SFINAE: compile error
// WITH SFINAE: template silently discarded, compiler looks for other overloads
// since no overload found `error: no matching function for call to ‘front(int)’` will be thrown 
```

SFINAE is meant to be used when code has multiple templates. If one fails, it is silently discarded, but the compiler successfully finds a different one to use instead, resulting in zero compiler errors.

### Where SFINAE Applies and Does Not Apply

SFINAE applies to substitution failures in the immediate context — the template *parameter list* and *function signature*.
  * Immediate Context (SFINAE applies): This includes the template parameter list, the function's return type, and the function's parameter types. If a substitution error happens here, the compiler silently drops the template.
SFINAE does NOT apply to errors in the function body:
  * The Function Body (SFINAE does NOT apply): Once the compiler successfully passes the function signature and decides to instantiate the function, any error found inside the `{ ... }` curly braces is a hard compilation error. 

```cpp
template<typename T>
void process(T x) {
    x.nonExistent();   // body error — NOT SFINAE — always compile error
}
```


SFINAE stands for Substitution Failure Is Not An Error. When the C++ compiler instantiates a template, it substitutes the template arguments into the template's signature and body. If that substitution results in an invalid type or expression (e.g., trying to access a member of a type that doesn't have it), the compiler does not halt with a hard compilation error. Instead, it simply removes that specific overload from the set of candidate functions and moves on to check other overloads.


## std::enable_if — Conditional Enabling

### What It Is

`std::enable_if<Condition, T>` either:
- Provides `type = T` when Condition is true
- Produces a substitution failure when Condition is false

`std::enable_if` is a standard library tool designed to conditionally control whether a function template or class template is available for the compiler to use. It relies directly on the SFINAE (Substitution Failure Is Not An Error) mechanism.

### Internal Implementation Structure

The standard library implements `std::enable_if` using a primary template alongside a partial template specialization.

```cpp
// Primary template (matched when the condition is false)
template <bool B, typename T = void>
struct enable_if {};

// Partial specialization (matched when the condition is true)
template <typename T>
struct enable_if<true, T> {
    using type = T;
};
```

* **When the condition evaluates to `true`**: The compiler selects the specialized struct (`enable_if<true, T>`). This struct contains a nested type definition: `using type = T;`. Accessing `std::enable_if<true, T>::type` successfully resolves to the type `T`.
* **When the condition evaluates to `false`**: The compiler selects the primary struct (`enable_if<false, T>`). This struct is entirely empty and contains no member named `type`. Attempting to access `std::enable_if<false, T>::type` causes a substitution failure. Because this failure occurs in the immediate context of a template signature, SFINAE triggers, and the compiler silently discards that specific template candidate without throwing a compilation error.

### Usage Patterns

* integral values   :   `int`, `long`, `char`, ..
* non-integral type :   `float`, `double`, `class`

### Understanding `std::enable_if_t`

`std::enable_if_t` is a convenient type alias introduced in C++14 to shorten syntax. It avoids the repetitive need to write `typename std::enable_if<...>::type`.

It takes two template parameters:

1. **`B` (The Boolean Condition)**: Evaluates to either `true` or `false`. If `true`, the template is retained. If `false`, SFINAE removes the template.
2. **`T` (The Result or Fallback Type)**: Specifies the underlying type the expression evaluates to when `B` is `true`. If omitted, it defaults to `void`.

### Integration Patterns in Templates

When applying `std::enable_if_t` to functions, developers place the check in one of two standard locations within the template signature.

#### Pattern A: The Return Type Approach

The condition check is embedded directly into the function's return type.

```cpp
#include <type_traits>

template<typename T>
std::enable_if_t<std::is_integral_v<T>, T> doubled(T value) {
    return value * 2;
}
```
* **How it works**: If `T` is an integer type (such as `int`), `std::is_integral_v<T>` is `true`. `std::enable_if_t` evaluates to the second parameter (`T`). The function signature becomes `T doubled(T value)`.
* **Why it fails for non-integers**: If `T` is a floating-point type (such as `double`), `std::is_integral_v<T>` is `false`. The evaluation yields `std::enable_if_t<false, double>`, which contains no nested `type`. SFINAE discards the template.

#### Pattern B: The Template Parameter List Approach

The condition check is placed inside the template parameter list as a default-valued non-type template parameter, leaving the function return type and parameter list completely clean.

```cpp
template<
    typename T,
    std::enable_if_t<std::is_integral_v<T>, int> = 0
>
T doubled(T value) {
    return value * 2;
}
```

* **How it works**: The second template parameter evaluates to an `int` with a default value of `0` when the condition is `true`.
* **Role of the fallback type (`int`) and value (`0`)**: `std::enable_if_t` requires a concrete type to resolve to when the condition is true. `int` serves as a placeholder type, and `= 0` provides a valid default argument for that non-type template parameter.
* **Why it is preferred**: This approach keeps the function's actual return type readable and unpolluted by type trait logic.

The type `int` serves as the explicit type for the non-type template parameter being declared. In this pattern, the expression does not define a return type; instead, it declares a hidden template parameter that accepts a default value (`= 0`).

Omitting the `int` parameter causes `std::enable_if_t` to fall back to its default type, which is `void`.

```cpp
// If 'int' is omitted, it evaluates to void:
template<
    typename T,
    std::enable_if_t<std::is_integral_v<T>> = 0  // Expands to: void = 0
>

```

This substitution triggers a hard compilation error because the C++ language specification does not permit non-type template parameters to have the type `void`. Variables and template parameters cannot possess a `void` type.

Supplying `int` provides a legal, concrete type (`int = 0`) that the compiler accepts, allowing the template parameter mechanism to function correctly when the condition evaluates to `true`.

### Two Exclusive Overloads

`std::enable_if` enables the creation of multiple function overloads that share identical parameter types but execute different code paths depending on compile-time type traits.

```cpp
#include <iostream>
#include <type_traits>

// Overload 1: Active only when T is an integer
template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
void process(T value) {
    std::cout << "Integer path: " << value << "\n";
}

// Overload 2: Active only when T is a floating-point number
template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
void process(T value) {
    std::cout << "Floating-point path: " << value << "\n";
}

int main() {
    process(42);     // Matches Overload 1 (Integer)
    process(3.14);   // Matches Overload 2 (Floating-point)
}

```

* **Why this pattern exists**: Without `std::enable_if`, writing two function templates with identical signatures (`void process(T value)`) creates an ambiguous redefinition error. `std::enable_if` ensures that for any given type `T`, exactly one overload has a valid substitution while the other fails cleanly via SFINAE.


### Why enable_if Is Replaced by Concepts in C++20

While `std::enable_if` is standard in pre-C++20 codebases, it introduces verbose syntax, complex compiler error messages, and boilerplate code. C++20 replaces this mechanism with **Concepts**, which provide direct language support for constraining templates.

```cpp
// C++20 Concept equivalent
void process(std::integral auto value) {
    std::cout << "Integer path: " << value << "\n";
}

void process(std::floating_point auto value) {
    std::cout << "Floating-point path: " << value << "\n";
}

```

`std::enable_if` is still encountered in pre-C++20 codebases.
