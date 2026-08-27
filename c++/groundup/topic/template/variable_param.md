## Variadic Templates

A variadic template accepts any number of template arguments — zero or more. This is the mechanism behind `std::tuple`, `std::make_unique`, and type-safe forwarding.

### Parameter Pack

`typename... Args` declares a pack. `Args...` expands it.

```cpp
template<typename... Args>
void print(Args... args) { /* ... */ }

print();                      // Args = (empty)
print(1);                     // Args = (int)
print(1, 2.0, "hello");       // Args = (int, double, const char*)
```

### sizeof... — Count of Pack Elements

```cpp
template<typename... Args>
void countArgs(Args... args) {
    std::cout << sizeof...(Args) << "\n";   // count of types
    std::cout << sizeof...(args) << "\n";   // count of values — same number
}

countArgs(1, 2.0, "hi");   // prints 3 and 3
```

### Recursive Variadic Template

The classic pattern — peel off the first argument, recurse on the rest:

```cpp
// Base case — no arguments
void print() { std::cout << "\n"; }

// Recursive case
template<typename First, typename... Rest>
void print(First first, Rest... rest) {
    std::cout << first << " ";
    print(rest...);   // recurse with remaining
}

print(1, 2.0, "hello", true);
// 1 → print(2.0, "hello", true)
// 2 → print("hello", true)
// hello → print(true)
// true → print()
// newline
// Output: 1 2 hello 1
```

### Pack Expansion

`pattern...` expands to a comma-separated list with the pattern applied to each element:

```cpp
template<typename... Args>
void forward_all(Args&&... args) {
    process(std::forward<Args>(args)...);
    // expands to: process(std::forward<T1>(a1), std::forward<T2>(a2), ...)
}
```

Pack expansion contexts:

```cpp
template<typename... Args>
struct Tuple {
    std::tuple<Args...> data;   // in template argument list
};

template<typename... Args>
void multi_construct(Args... args) {
    auto arr = std::array{args...};   // in initializer list
}
```

## Fold Expressions (C++17)

Fold expressions eliminate recursive variadic templates for simple operations by applying a binary operator across a parameter pack directly.

### Four Forms

| Form | Expansion |
|---|---|
| `(pack op ...)` | Unary right fold: `a1 op (a2 op (a3 op aN))` |
| `(... op pack)` | Unary left fold: `((a1 op a2) op a3)` |
| `(pack op ... op init)` | Binary right fold with initial value |
| `(init op ... op pack)` | Binary left fold with initial value |

### Examples

```cpp
// Sum all arguments
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);   // unary right fold
}
sum(1, 2, 3, 4, 5);   // 15

// Print with spaces — replaces the recursive version
template<typename... Args>
void print(Args... args) {
    ((std::cout << args << " "), ...);   // fold over comma operator
    std::cout << "\n";
}
print(1, 2.0, "hello");   // "1 2 hello "

// Logical AND of all
template<typename... Args>
bool all_true(Args... args) {
    return (... && args);   // left fold
}
all_true(true, true, true);    // true
all_true(true, false, true);   // false

// Sum with initial value
template<typename... Args>
auto sum_from_100(Args... args) {
    return (100 + ... + args);   // binary left fold, init = 100
}
sum_from_100(1, 2, 3);   // 106
```

### Empty Pack Behavior

```cpp
// Unary fold on empty pack — compile error for most operators
// Binary fold — initial value returned when pack is empty
template<typename... Args>
auto safe_sum(Args... args) {
    return (0 + ... + args);   // binary: empty pack gives 0
}
safe_sum();          // 0
safe_sum(1, 2, 3);   // 6
```

## Index-Based Access via std::tuple or Arrays
If need random access to the $i$-th element of a parameter pack, pack them into a `std::tuple` or a braced initializer list and query by index using `std::get<I>`.
```cpp
template<std::size_t I, typename... Args>
auto getElement(Args&&... args) {
    // Pack arguments into a tuple and retrieve by index
    return std::get<I>(std::forward_as_tuple(args...));
}

// Usage:
// getElement<1>(10, "hello", 3.14); // returns "hello"
```