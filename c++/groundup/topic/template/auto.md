## Abbreviated Function Templates (C++20)

### What They Are

`auto` as a function parameter type makes the function automatically a template. Each `auto` parameter is an independent anonymous type parameter.

```cpp
// Traditional:
template<typename T>
T square(T x) { return x * x; }

// Abbreviated — identical behavior:
auto square(auto x) { return x * x; }
```

### With Concepts — The Cleanest Form

```cpp
auto doubled(std::integral auto x) { return x * 2; }

auto myMax(std::totally_ordered auto a, std::totally_ordered auto b) {
    return a > b ? a : b;
}

// Multiple auto — each is independent type
auto add(auto a, auto b) { return a + b; }
add(1, 2);       // T1 = int,    T2 = int
add(1, 2.0);     // T1 = int,    T2 = double
```

### Named vs Anonymous — When to Use Each

```cpp
// auto — each parameter is independent type
// a and b can be different types
auto max(auto a, auto b) { return a > b ? a : b; }
max(3, 4.0);   // a = int, b = double — compares across types

// named T — same type enforced
template<typename T>
T max(T a, T b) { return a > b ? a : b; }
// max(3, 4.0);  // COMPILE ERROR: conflicting deduction T=int vs T=double
```

Use named template parameters when the same type must appear in multiple positions. Use abbreviated syntax for simple, unconstrained parameters.