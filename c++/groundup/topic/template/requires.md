
# requires Expressions (C++20)

A `requires` expression tests whether expressions or types are valid for a given type. Returns true or false at compile time. The building block for concepts

## Two Different Things Share One Keyword

Before any example makes sense, one fact must be separated out: the word `requires` is used for **two grammatically different constructs** in C++20. They look similar and can even be nested inside each other, which is the root of most confusion.

```
requires-CLAUSE       — attaches a constraint to a template
                        does NOT have braces {}
                        example: template<typename T> requires Integral<T>

requires-EXPRESSION    — a value-producing expression that evaluates to
                        true or false at compile time
                        HAS braces {} containing a body
                        example: requires(T t) { t.size(); }
```

* A requires-**clause** is a gate — it decides whether a template participates in overload resolution.
* A requires-**expression** is a boolean-valued expression — it can be used *inside* a requires-clause, or anywhere a compile-time bool is needed (like inside a `concept` definition).



## The requires-Expression — Full Anatomy

```
requires (parameter-list) { requirement-sequence }
   ↑            ↑                    ↑
 keyword    optional            one or more
                                requirements,
                                each ending in ;
```

**The most important fact about a requires-expression:** nothing inside it ever executes. No object is constructed. No function is actually called. The compiler only checks whether the code **would compile** if it were written — it never runs it. The entire thing is evaluated purely at compile time and produces a single `bool`: `true` if every requirement inside is satisfied, `false` otherwise.

```cpp
requires(T t) { t.size(); }

requires(T t) { 5 > 3; }
```

This is a simple requirement. The only check: does `5 > 3` compile?
`5 > 3` is valid C++ for every type T imaginable. It always compiles. So this requires-expression always evaluates to true — for every type T.

The entire requires-expression is a compile-time type check. The compiler reads `5 > 3` and asks one question only:
`"Is this a valid C++ expression?" → YES → requirement satisfied`

except the "evaluation" here happens entirely during compilation, by checking whether code compiles — not by running any code.

The value true that `5 > 3` produces at runtime is never computed. No CPU instruction runs. The compiler is only checking the grammar and types — not evaluating values.

```cpp
template<typename T>
concept AlwaysTrue = requires(T t) {
    5 > 3;
};

template<typename T>
concept AlwaysTrue1 = requires {
    3 > 5;
};

static_assert(AlwaysTrue<int>);         // true
static_assert(AlwaysTrue<std::string>); // true
static_assert(AlwaysTrue<void*>);       // true

static_assert(AlwaysTrue1<int>);         // true
static_assert(AlwaysTrue1<std::string>); // true
static_assert(AlwaysTrue1<void*>);       // true
```
It is valid. It is useless. It constrains nothing.

### Simple Requirement — Expression Must Be Valid

```cpp
template<typename T>
concept HasSize = requires(T c) {
    c.size();   // T must have size() — no return type check
};
```

### Type Requirement — Type Must Exist

```cpp
template<typename T>
concept HasValueType = requires {
    typename T::value_type;
};
```

### Compound Requirement — Valid AND Return Type Constrained

```cpp
template<typename T>
concept Sizeable = requires(T c) {
    { c.size() } -> std::convertible_to<std::size_t>;
    // size() must exist AND return type must convert to size_t
};
```

### Nested Requirement — Boolean Condition Inside

```cpp
template<typename T>
concept SmallType = requires {
    requires sizeof(T) <= 8;   // T must fit in 8 bytes
};
```

### Standalone requires Clause

```cpp
template<typename T>
    requires (sizeof(T) <= 8 && std::is_trivially_copyable_v<T>)
void fastCopy(T* dest, const T* src, std::size_t n) {
    std::memcpy(dest, src, n * sizeof(T));
}
```
