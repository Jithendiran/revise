# Template Specialization — Primary, Full, and Partial

## The Problem — One Template Cannot Handle Every Type Equally

A primary template defines the general behavior for all types. But some types need different behavior — either because the general logic does not work for them, or because a more efficient approach exists for them specifically.

```cpp
template<typename T>
struct Storage {
    T data;
    void print() {
        std::cout << data << "\n";
    }
};
```

This works for `int`, `double`, `std::string`. But for `bool`, storing a full `int`-sized value for a single bit is wasteful. For `const char*`, printing works but comparison and copying are broken. Different types need different implementations.

Specialization is the mechanism that provides those different implementations while keeping the general template for everything else.

## The Three Forms — What Each One Is
```
Primary template → the general blueprint, handles all types by default
Full specialization → replaces the template for ONE exact type combination
Partial specialization → replaces the template for a PATTERN of types
```

All three work together. The compiler selects the most specific match for any given set of template arguments.

## A Single Example Showing All Three Together

```cpp
#include <iostream>
#include <string>

// ─────────────────────────────────────────────────────────────
// PRIMARY TEMPLATE — handles all types not covered by a specialization
// ─────────────────────────────────────────────────────────────
template<typename T, typename U>
struct Converter {
    static void convert(T from, U to) {
        std::cout << "[primary]   converting " << from
                  << " → " << to << "\n";
    }
};

// ─────────────────────────────────────────────────────────────
// FULL SPECIALIZATION — handles EXACTLY <int, double>
// template<> with empty <> — no generic parameters left
// ─────────────────────────────────────────────────────────────
template<>
struct Converter<int, double> {
    static void convert(int from, double to) {
        std::cout << "[full spec] int→double: "
                  << from << " → " << to << "\n";
    }
};

// ─────────────────────────────────────────────────────────────
// PARTIAL SPECIALIZATION — handles <T, std::string> for ANY T
// T is still generic — only U is locked to std::string
// ─────────────────────────────────────────────────────────────
template<typename T>
struct Converter<T, std::string> {
    static void convert(T from, std::string to) {
        std::cout << "[partial]   ?→string:  "
                  << from << " → " << to << "\n";
    }
};

int main() {
    Converter<double, float>::convert(1.5, 2.0f);    // primary
    Converter<int, double>::convert(42, 3.14);        // full specialization
    Converter<int, std::string>::convert(99, "hi");   // partial specialization
    Converter<bool, std::string>::convert(true, "x"); // partial specialization
}
```

Output:

```
[primary]   converting 1.5 → 2
[full spec] int→double: 42 → 3.14
[partial]   ?→string:  99 → hi
[partial]   ?→string:  1 → x
```

## How the Compiler Selects — The Priority Order

When a template is used, the compiler checks all three forms and picks the most specific match:
```
Priority 1 — Full specialization:
Does an exact match exist for this exact combination of types?
YES → use it
NO  → move to priority 2

Priority 2 — Partial specialization:
Does a pattern match exist that is more specific than the primary?
YES → use the most specific partial specialization
NO  → move to priority 3

Priority 3 — Primary template:
No specialization matched — generate from the primary template
```

```cpp
Converter<int, double>      // Priority 1: exact match <int, double> → full spec
Converter<int, std::string> // Priority 2: matches <T, std::string> → partial spec
Converter<double, float>    // Priority 3: no match → primary template
```
## Primary Template

The primary template is the general blueprint. It handles every type combination that has no more specific specialization.

```cpp
template<typename T, typename U>   // all parameters generic
struct Converter {
    static void convert(T from, U to) {
        std::cout << "[primary]\n";
    }
};
```

**Rules:**
- Must be declared before any specialization
- Every specialization depends on the primary existing — a specialization cannot stand alone without a primary template
- The primary is used as a fallback when no specialization matches

## Full Specialization

Full specialization locks in every template parameter. The `template<>` line has an empty parameter list because no generic parameters remain all arguments are fixed.

```cpp
template<>                          // empty — nothing generic left
struct Converter<int, double> {     // both arguments locked: int AND double
    static void convert(int from, double to) {
        std::cout << "[full spec]\n";
    }
};
```

**Rules:**
- `template<>` with empty brackets is mandatory
- Every argument in the angle brackets after the class name is a concrete type
- Cannot be declared without a primary template already existing
- The full specialization is a completely independent class — it inherits nothing from the primary template
- Only used for the exact combination `<int, double>` — nothing else

**When to use:**
- The general logic is incorrect for one specific type combination
- A dramatically different or faster implementation exists for one type
- The general template cannot compile for a specific type (e.g., `bool`)

**Supports**
1. Classes and Structs: Fully supported for completely specified types (e.g., `template<> class MyClass<int>`).
2. Functions: Supported (e.g., `template<> void print<int>(int val)`), though generally discouraged in modern C++ because function templates do not participate cleanly in overload resolution.
3. Variables: Fully supported for variable templates (e.g., `template<> constexpr double pi<double> = 3.14159;`).

```cpp
// Example: bool needs completely different storage
template<typename T>
struct Storage {
    T data;                                // general: stores T directly
    void print() { std::cout << data; }
};

template<>
struct Storage<bool> {                     // full spec: bool stored as a flag
    unsigned char flag : 1;
    void print() { std::cout << (flag ? "true" : "false"); }
    void toggle() { flag = !flag; }        // extra method — only for bool
};
```

//--------------------------------------------------------- TODO

## Partial Specialization

Partial specialization locks in some parameters while leaving others generic. The `template<...>` line still has parameters — the ones that remain generic.

```cpp
template<typename T>                // T is still generic
struct Converter<T, std::string> {  // only U is locked to std::string
    static void convert(T from, std::string to) {
        std::cout << "[partial]\n";
    }
};
```

**Rules:**
- The parameters in `template<...>` are the ones still generic
- The parameters in `<...>` after the class name show the pattern being matched — some are concrete, some use the remaining generic parameters
- More than one partial specialization can exist for the same template

**When to use:**
- The same alternative logic works for a whole family of types (all pointers, all types paired with `std::string`, etc.)
- Detecting type patterns at compile time (is it a pointer? a reference? a pair of same types?)

**Support**
1. Classes and Structs: Fully supported, allowing to specialize for a subset of types (e.g., `template<typename T> class MyClass<T*>`).
2. Functions: Not supported. C++ does not allow partial specialization of function templates. To achieve similar behavior, rely on regular function overloading or wrap the function inside a helper class template.
3. Variables: Fully supported for variable templates (e.g., specializing a variable template for pointer types).

```cpp
// Partial spec for ALL pointer types
template<typename T>
struct Storage<T*> {
    T* ptr;
    void print() { std::cout << "pointer → " << *ptr; }
};

// Partial spec when both types are the same
template<typename T>
struct Converter<T, T> {
    static void convert(T from, T to) {
        std::cout << "[same type]\n";
    }
};
```

### Why Function Partial Specialization Is Disallowed
Function templates omit partial specialization primarily because function overloading already solves this problem natively.
* Overload Resolution Complexity: Allowing partial specialization alongside function overloading would make overload resolution exceptionally convoluted. The compiler would face severe challenges determining whether to select a function overload or a partially specialized template, frequently resulting in unresolvable ambiguities.

### How Variable Template Partial Specialization Works
Introduced in C++14, variable templates allow to parameterize constants. Much like class templates, variable templates fully support partial specialization to target specific type patterns rather than exact matches.

```cpp
// 1. Primary variable template for general types
template<typename T>
int data_width = sizeof(T);

// 2. Partial specialization for any pointer type
template<typename T>
int data_width<T*> = sizeof(void*);

// 3. Full specialization for a specific type
template<>
int data_width<double> = 16;
```
In practice, evaluating `data_width<int>` resolves to the primary template (`sizeof(int)`), while evaluating `data_width<int*>` or `data_width<char*>` matches the partial specialization for any pointer pattern (`T*`), returning `sizeof(void*)`. This capability mirrors class template partial specialization, letting compile-time values dynamically adapt to entire families of types.