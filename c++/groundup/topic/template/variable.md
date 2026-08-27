## Variable Templates (C++14)

A variable template generates a variable or constant for each instantiation. This is the cleanest way to express type-dependent constants.

Variable templates do not support template argument deduction (CTAD) from their usage expressions the way function templates do. So need a explicit template variable

**Why it can't dedude?**
When function template is used parameter argumanet is passed into it. The compiler looks at the type of that argument to figure out what `T` should be.
```cpp
template <typename T>
void print(T x) { /* ... */ }

print(5); // The compiler sees the number 5, knows it's an int, and decides: "Ah, T must be int!"
```
The compiler can deduce the type because the argument is fed directly into the function.

A variable is just a storage box or a value. It doesn't take inputs or parameters the way a function does.

```cpp
template<typename T>
constexpr T pi = static_cast<T>(3.14159265358979323846);

double p = pi * 2;
// 1st pi * 2 will be executed then content will be stored to `p` 
```
1. The compiler looks at `pi`. At this exact moment, `pi` is just a template blueprint, not an actual variable yet.
2. The compiler needs to create (instantiate) `pi` before it can do any math with it.
3. But `pi` has no arguments attached to it. The `* 2` happens after `pi` already exists.

The compiler cannot look ahead at the `* 2`

### Problem Before C++14

```cpp
// C++11 — verbose class template workaround
template<typename T>
struct pi_value {
    static constexpr T value = static_cast<T>(3.14159265358979);
};

double pi_d = pi_value<double>::value;
```

### Variable Template Syntax

```cpp
template<typename T>
constexpr T pi = static_cast<T>(3.14159265358979323846);

double area = pi<double> * r * r;
float  area = pi<float>  * r * r;
```

### Standard Library _v Helpers (C++17)

```cpp
// Before C++17 — verbose:
std::is_integral<int>::value        // true
std::is_same<int, int>::value       // true

// C++17 variable templates — concise:
std::is_integral_v<int>             // true
std::is_same_v<int, int>            // true
std::is_pointer_v<int*>             // true
std::is_const_v<const int>          // true



template <typename T>
void print_integer_only(T val) {
    // Check at compile-time if T is an integer type
    static_assert(std::is_integral_v<T>, "Error: This function only accepts whole numbers (integers)!");

    std::cout << "Valid integer: " << val << "\n";
}

int main() {
    print_integer_only(10);      // Works fine! 'int' is integral.
    print_integer_only(3.14);    // COMPILE ERROR! Stops you right here.
}
```

### Compile-Time Computation With Variable Templates

```cpp
template<std::size_t N>
constexpr std::size_t factorial = N * factorial<N - 1>;

template<>
constexpr std::size_t factorial<0> = 1;   // full specialization — base case

constexpr std::size_t f5 = factorial<5>;   // 120 — compile time
constexpr std::size_t f3 = factorial<3>;   // 6
```

## Alias Templates

An alias template is a `using` declaration with template parameters. It creates a parameterized name for an existing type.

### Problem

```cpp
// Repeated verbose type
std::map<std::string, int>         scores;
std::map<std::string, double>      prices;
std::map<std::string, std::string> names;
```

### Alias Template Solution

```cpp
template<typename Value>
using StringMap = std::map<std::string, Value>;

StringMap<int>         scores;   // = std::map<std::string, int>
StringMap<double>      prices;   // = std::map<std::string, double>
StringMap<std::string> names;    // = std::map<std::string, std::string>
```

`StringMap<int>` IS `std::map<std::string, int>` — same type, no new type created.

### Common Patterns

```cpp
// Parameterized function type
template<typename R, typename... Args>
using Func = std::function<R(Args...)>;

Func<int, int, int> adder = [](int a, int b) { return a + b; };

// Remove const shorthand
template<typename T>
using RemoveConst = std::remove_const_t<T>;
```

### Alias Template vs typedef

```cpp
// typedef — cannot be parameterized
typedef std::map<std::string, int> StringIntMap;   // fixed types only

// Alias template — parameterized
template<typename V>
using StringMapTo = std::map<std::string, V>;
```